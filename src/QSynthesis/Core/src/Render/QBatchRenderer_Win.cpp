#include "QBatchRenderer.h"

#ifdef Q_OS_WINDOWS

#include "Utau/Config/UtaConstants.h"
#include "Utau/Utils/UtaTranslator.h"

#include <QDir>
#include <QFile>

#define _NS(path) QDir::toNativeSeparators(path)

bool QBatchRenderer::generateWindowsBatch(const QList<RenderArgs> &args) const {
    // Step 1: Call Notes-Area
    // Step 2: Fetch all parameters from Note of GUI Note
    // Step 3: If in Mode2, compute pitch curves string by reference of time values
    // Step 4: Set parameters to WavtoolArgs and ResamplerArgs class
    // Step 5: If in command line, write the parameters to batch file
    // Step 6: Execute batch file

    // Generate temp.bat
    QFile tempFile(tempBat);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream fs(&tempFile);

    double tempo =
        (args.isEmpty() ? Utau::DEFAULT_VALUE_TEMPO : args.front().resamplerArgs.tempo());

    fs << "@set loadmodule=" << Qt::endl;
    fs << "@set tempo=" << tempo << Qt::endl;
    fs << "@set samples=" << 44100 << Qt::endl;
    fs << "@set oto=" << _NS(voice) << Qt::endl;
    fs << "@set tool=" << _NS(wavtool) << Qt::endl;
    fs << "@set resamp=" << _NS(resampler) << Qt::endl;
    fs << "@set output=" << temp << Qt::endl;
    fs << "@set helper=" << tempBat << Qt::endl;
    fs << "@set cachedir=" << _NS(cache) << Qt::endl;
    fs << "@set flag="
       << "\"" << flags << "\"" << Qt::endl;
    fs << "@set env="
       << "0 5 35 0 100 100 0" << Qt::endl;
    fs << "@set stp=" << 0 << Qt::endl;

    fs << Qt::endl;
    fs << "@del \"%output%\" 2>nul" << Qt::endl;
    fs << "@mkdir \"%cachedir%\" 2>nul" << Qt::endl;
    fs << Qt::endl;
    fs << Qt::endl;

    int count = args.size();
    for (int i = 0; i < count; ++i) {
        ResamplerArgs res = args.at(i).resamplerArgs;
        WavtoolArgs wav = args.at(i).wavtoolArgs;

        QOtoItem aGenon = res.genonSettings();
        QOtoItemBrief aCorrect = res.correctGenon();

        if (wav.isRest()) {
            fs << "@\"%tool%\" \"%output%\" \"%oto%\\R.wav\" 0";
            fs << " " << wav.outDuration(); // Static Duration
            fs << " 0 0";
            fs << Qt::endl;
        } else {
            fs << "@echo " << UtaTranslator::LoadingBar(i + 1, count) << Qt::endl;

            fs << "@set params=" << res.paramsArguments().join(" ") << Qt::endl;
            fs << "@set flag="
               << "\"" << res.flags() << "\"" << Qt::endl;
            fs << "@set env=" << wav.envArguments().join(" ") << Qt::endl;

            fs << "@set stp=" << wav.startPoint() << Qt::endl;
            fs << "@set vel=" << res.velocity() << Qt::endl;
            fs << "@set temp="
               << "\"%cachedir%\\" << cache << "\"" << Qt::endl;

            // Transmit parameters
            fs << "@call"
               << " "
               << "\"%helper%\"";
            fs << " "
               << "\"%oto%\\" << _NS(res.inFile().mid(voice.size() + 1))
               << "\""; // Original file name

            fs << " " << res.toneName();        // Tone Name
            fs << " " << wav.outDuration();     // Static Duration
            fs << " " << aCorrect.PreUtterance; // PreUtterance (May not used)
            fs << " " << aGenon.Offset;         // Offset (Left Blue Area)
            fs << " " << res.realLength();
            fs << " " << aGenon.Constant; // Consonant (Constant Area)
            fs << " " << aGenon.Blank;    // Blank (Right Blue Area)
            fs << " " << res.sequence();  // Sequence Number

            fs << Qt::endl;
        }
        fs << Qt::endl;
    }

    fs << Qt::endl;

    fs << "@if not exist \"%output%.whd\" goto E" << Qt::endl;
    fs << "@if not exist \"%output%.dat\" goto E" << Qt::endl;
    fs << "copy /Y \"%output%.whd\" /B + \"%output%.dat\" /B \"%output%\"" << Qt::endl;
    fs << "del \"%output%.whd\"" << Qt::endl;
    fs << "del \"%output%.dat\"" << Qt::endl;
    fs << ":E" << Qt::endl;

    tempFile.close();

    return true;
}

bool QBatchRenderer::generateWindowsHelper() const {
    // Generate temp_helper.bat
    QFile helperFile(tempBat);
    if (!helperFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream fs(&helperFile);

    fs << "@if exist %temp% goto A" << Qt::endl;
    fs << "@if exist \"%cachedir%\\%9_*.wav\" del \"%cachedir%\\%9_*.wav\"" << Qt::endl;
    fs << "@\"%resamp%\" %1 %temp% %2 %vel% %flag% %5 %6 %7 %8 %params%" << Qt::endl;
    fs << ":A" << Qt::endl;
    fs << "@\"%tool%\" \"%output%\" %temp% %stp% %3 %env%" << Qt::endl;

    helperFile.close();

    return true;
}

#endif
