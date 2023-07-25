#ifndef CHORUSKIT_JSMACROS_H
#define CHORUSKIT_JSMACROS_H

#define JS_PROP_ASSERT(obj, prop, type) (obj.contains(#prop) && obj.value(#prop).canConvert(QVariant::type))
#define JS_PROP_AS(obj, prop, type) (obj.value(#prop).to##type())

#define JS_PROP_OPTIONAL_IF(obj, prop, type)                                                                                  \
    if (JS_PROP_ASSERT(obj, prop, type)) {                                                                    \
        auto prop##Prop = JS_PROP_AS(obj, prop, type);
#define JS_PROP_OPTIONAL_ELSE                                                                                            \
    }                                                                                                                  \
    else {
#define JS_PROP_OPTIONAL_ENDIF }
#define JS_PROP_REQUIRED(obj, prop, type)                                                                                     \
    if (!JS_PROP_ASSERT(obj, prop, type))                                                                     \
        return false;                                                                                                  \
    auto prop##Prop = JS_PROP_AS(obj, prop, type)

#endif // CHORUSKIT_JSMACROS_H
