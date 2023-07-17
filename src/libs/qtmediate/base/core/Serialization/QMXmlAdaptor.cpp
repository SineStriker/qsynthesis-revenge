#include "QMXmlAdaptor.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QXmlStreamReader>

QJsonObject QMXmlAdaptorElement::toObject() const {
    QJsonObject obj;
    obj.insert("name", name);
    if (!value.isEmpty()) {
        obj.insert("value", value);
    } else {
        QJsonArray arr;
        for (const auto &child : qAsConst(children)) {
            arr.append(child->toObject());
        }
        obj.insert("children", arr);
    }
    QJsonObject props;
    for (auto it = properties.begin(); it != properties.end(); ++it) {
        props.insert(it.key(), it.value());
    }
    obj.insert("properties", props);
    return obj;
}

QMXmlAdaptorElement QMXmlAdaptorElement::fromObject(const QJsonObject &obj) {
    QMXmlAdaptorElement res;
    QJsonObject::ConstIterator it;

    it = obj.find("name");
    if (it == obj.end() || !it->isString()) {
        return {};
    }
    res.name = it->toString();

    it = obj.find("properties");
    if (it != obj.end() && it->isObject()) {
        const auto &props = it->toObject();
        for (auto it2 = props.begin(); it2 != props.end(); ++it2) {
            if (it2->isDouble()) {
                res.properties.insert(it2.key(), QString::number(it2->toDouble()));
            } else {
                res.properties.insert(it2.key(), it2->toString());
            }
        }
    }

    it = obj.find("value");
    if (it != obj.end() && it->isString()) {
        res.value = it->toString();
        return res;
    }

    it = obj.find("children");
    if (it != obj.end() && it->isArray()) {
        const auto &children = it->toArray();
        for (const auto &child : children) {
            if (!child.isObject()) {
                continue;
            }
            auto tmp = fromObject(child.toObject());
            if (tmp.name.isEmpty()) {
                continue;
            }
            res.children.append(Ref::create(std::move(tmp)));
        }
    }

    return res;
}

void QMXmlAdaptorElement::writeXml(QXmlStreamWriter &writer) const {
    const auto &ele = *this;

    if (ele.name.isEmpty()) {
        return;
    }
    writer.writeStartElement(ele.name);

    for (auto it = ele.properties.begin(); it != ele.properties.end(); ++it) {
        writer.writeAttribute(it.key(), it.value());
    }

    if (!ele.value.isEmpty()) {
        writer.writeCharacters(ele.value);
        goto out;
    }

    for (const auto &child : ele.children) {
        child->writeXml(writer);
    }

out:
    writer.writeEndElement();
}

QMXmlAdaptor::QMXmlAdaptor() {
}

QMXmlAdaptor::~QMXmlAdaptor() {
}

bool QMXmlAdaptor::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader reader(&file);
    bool success = true;

    std::list<QPair<QMXmlAdaptorElement::Ref, int>> stack;

    int curLevel = 0;
    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType nType = reader.readNext();
        switch (nType) {
            case QXmlStreamReader::StartDocument: {
                // qDebug() << "********** Start XML ********** ";
                // QString strVersion = reader.documentVersion().toString();
                // QString strEncoding = reader.documentEncoding().toString();
                // bool bAlone = reader.isStandaloneDocument();
                // qDebug() << QString("Version: %1  Encoding: %2  Standalone: %3")
                //                 .arg(strVersion, strEncoding, QString::number(bAlone));
                break;
            }
            case QXmlStreamReader::StartElement: {
                auto ele = QMXmlAdaptorElement::Ref::create();

                // Store name
                ele->name = reader.name().toString();

                // Store attributes
                const auto &attrs = reader.attributes();
                for (const auto &attr : attrs) {
                    ele->properties.insert(attr.name().toString(), attr.value().toString());
                }

                stack.push_back(qMakePair(ele, curLevel++));
                break;
            }
            case QXmlStreamReader::EndElement: {
                curLevel--;
                QList<QMXmlAdaptorElement::Ref> tmpStack;
                while (!stack.empty()) {
                    const auto &top = stack.back();
                    if (top.second != curLevel) {
                        tmpStack.prepend(top.first);
                        stack.pop_back();
                        continue;
                    }
                    if (top.first->name != reader.name()) {
                        success = false;
                    } else {
                        top.first->children = std::move(tmpStack);
                    }
                    break;
                }
                break;
            }
            case QXmlStreamReader::Characters: {
                if (stack.empty()) {
                    success = false;
                } else {
                    const auto &top = stack.back();

                    // Store text
                    auto val = reader.text().trimmed();
                    if (!val.isEmpty()){
                        top.first->value = val.toString();
                    }
                }
                break;
            }
            case QXmlStreamReader::EndDocument: {
                // qDebug() << QString::fromLocal8Bit("********** End XML ********** ");
                break;
            }
            default:
                break;
        }
        if (!success) {
            break;
        } else if (reader.hasError()) {
            success = false;
            break;
        }
    }

    file.close();

    if (success && stack.size() == 1) {
        root = std::move(*stack.front().first);
        return true;
    }

    return false;
}

bool QMXmlAdaptor::save(const QString &filename) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    QByteArray data;
    QXmlStreamWriter writer(&data);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    root.writeXml(writer);
    writer.writeEndDocument();
    file.write(data);
    file.close();

    return true;
}