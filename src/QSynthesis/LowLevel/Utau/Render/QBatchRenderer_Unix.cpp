#include "QBatchRenderer.h"

#ifndef Q_OS_WINDOWS

#include "Utils/UtaTranslator.h"

#include <QDir>
#include <QFile>

bool QBatchRenderer::generateUnixBatch(const QList<RenderArgs> &args) const {
    // Generate temp.sh
    QFile tempFile(tempBat);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream fs(&tempFile);

    double tempo =
        (args.isEmpty() ? Utau::DEFAULT_VALUE_TEMPO : args.front().resamplerArgs.tempo());

    // Initial part of temp.sh
    fs << "#!/bin/bash" << Qt::endl;
    fs << Qt::endl;

    fs << "export loadmodule=" << Qt::endl;
    fs << "export tempo=" << tempo << Qt::endl;
    fs << "export samples=" << 44100 << Qt::endl;
    fs << "export oto="
       << "\'" << voice << "\'" << Qt::endl;
    fs << "export tool="
       << "\'" << wavtool << "\'" << Qt::endl;
    fs << "export resamp="
       << "\'" << resampler << "\'" << Qt::endl;
    fs << "export output="
       << "\'" << temp << "\'" << Qt::endl;
    fs << "export helper="
       << "\'"
       << "./temp_helper.sh"
       << "\'" << Qt::endl;
    fs << "export cachedir="
       << "\'" << cache << "\'" << Qt::endl;
    fs << "export flag="
       << "\'" << flags << "\'" << Qt::endl;
    fs << "export env="
       << "\'0 5 35 0 100 100 0\'" << Qt::endl;
    fs << "export stp=" << 0 << Qt::endl;

    fs << Qt::endl;
    fs << "rm -rf \"${output}\"" << Qt::endl;
    fs << "mkdir \"${cachedir}\"" << Qt::endl;
    fs << Qt::endl;
    fs << Qt::endl;

    int count = args.size();
    for (int i = 0; i < count; ++i) {
        ResamplerArgs res = args.at(i).resamplerArgs;
        WavtoolArgs wav = args.at(i).wavtoolArgs;

        QGenonSettings aGenon = res.genonSettings();
        QCorrectGenon aCorrect = res.correctGenon();

        if (wav.isRest()) {
            fs << "\"${tool}\" \"${output}\" \"${oto}/R.wav\" 0";
            fs << " " << wav.outDuration(); // Static Duration
            fs << " 0 0";
            fs << Qt::endl;
        } else {
            fs << "echo "
               << "\'" << UtaTranslator::LoadingBar(i + 1, count) << "\'" << Qt::endl;

            fs << "export params="
               << "\'" << res.paramsArguments().join(" ") << "\'" << Qt::endl;
            fs << "export flag="
               << "\'" << res.flags() << "\'" << Qt::endl;
            fs << "export env="
               << "\'" << wav.envArguments().join(" ") << "\'" << Qt::endl;

            fs << "export stp=" << wav.startPoint() << Qt::endl;
            fs << "export vel=" << res.velocity() << Qt::endl;
            fs << "export temp="
               << "\"${cachedir}/" << cache << "\"" << Qt::endl;

            // Transmit parameters
            fs << "\"${helper}\"";
            fs << " "
               << "\"${oto}/" << GetReativePath(res.inFile(), voice) << "\""; // Original file name

            fs << " " << res.toneName();        // Tone Name
            fs << " " << wav.outDuration();     // Static Duration
            fs << " " << aCorrect.PreUtterance; // PreUtterance (May not used)
            fs << " " << aGenon.mOffset;        // Offset (Left Blue Area)
            fs << " " << res.realLength();
            fs << " " << aGenon.mConstant; // Consonant (Constant Area)
            fs << " " << aGenon.mBlank;    // Blank (Right Blue Area)
            fs << " " << res.sequence();   // Sequence Number

            fs << Qt::endl;
        }
        fs << Qt::endl;
    }

    fs << Qt::endl;

    fs << "if [ -f \"${output}.whd\" ] && [ -f \"${output}.dat\" ]; then" << Qt::endl;
    fs << "\t"
       << "cat \"${output}.whd\" \"${output}.dat\" > \"${output}\"" << Qt::endl;
    fs << "\t"
       << "rm -f \"${output}.whd\"" << Qt::endl;
    fs << "\t"
       << "rm -f \"${output}.dat\"" << Qt::endl;
    fs << "fi" << Qt::endl;

    tempFile.close();

    return true;
}

bool QBatchRenderer::generateUnixHelper() const {
    // Generate temp_helper.sh
    QFile helperFile(helperBat);

    if (!helperFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return 0;
    }

    QTextStream fs(&helperFile);

    fs << "#!/bin/bash" << Qt::endl;
    fs << Qt::endl;

    fs << "if [ ! -f \"${temp}\" ]; then" << Qt::endl;
    fs << "\t"
       << "rm -rf \"${cachedir}/$9_*.wav\"" << Qt::endl;
    fs << "\t"
       << "\"${resamp}\" \"$1\" \"${temp}\" $2 ${vel} \"${flag}\" $5 $6 $7 $8 ${params}"
       << Qt::endl;
    fs << "fi" << Qt::endl;
    fs << "\"${tool}\" \"${output}\" \"${temp}\" ${stp} $3 ${env}" << Qt::endl;

    helperFile.close();

    return true;
}

#endif
