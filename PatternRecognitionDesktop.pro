#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T10:57:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Chart Pattern Finder"
TEMPLATE = app

win32:RC_FILE = ChartPatternFinder.rc

# Configure compiling and linking against libc++ instead of libstdc++
# This is needed to ensure consistency for all components, including
# the boost library.

macx: QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
macx: LIBS += -mmacosx-version-min=10.7 -stdlib=libc++
CONFIG += c++11

SOURCES += main.cpp\
    plot.cpp \
    griditem.cpp \
    legend.cpp \
    StockChartDateScaleDraw.cpp \
    StockChartPlotZoomer.cpp \
    StockChartPlotCurve.cpp \
    QDateHelper.cpp \
    MainWindow.cpp \
    PatternPlotCurve.cpp \
    PatternMatchTableView.cpp \
    instrumentList/InstrumentListTableView.cpp \
    PseudoTimeOHLCSample.cpp \
    BreakoutPlotMarker.cpp \
    instrumentList/InstrumentSelectionInfo.cpp \
    BreakdownPlotMarker.cpp \
    instrumentList/InstrumentList.cpp \
    StackedStockCharts.cpp \
    VolumePlot.cpp \
    VolumePlotCurve.cpp \
    VolumeYAxisScaleDraw.cpp \
    RegisterDialog.cpp \
    LicenseRegistration.cpp \
    SettingsHelper.cpp \
    instrumentList/InstrumentListTask.cpp \
    instrumentList/ReadQuotesFromFile.cpp \
    instrumentList/InstrumentListWorker.cpp \
    instrumentList/ScanPatternMatches.cpp

HEADERS  += \
    griditem.h \
    legend.h \
    plot.h \
    StockChartDateScaleDraw.h \
    StockChartPlotZoomer.h \
    StockChartPlotCurve.h \
    QDateHelper.h \
    MainWindow.h \
    PatternPlotCurve.h \
    PatternMatchTableView.h \
    instrumentList/InstrumentListTableView.h \
    PseudoTimeOHLCSample.h \
    BreakoutPlotMarker.h \
    instrumentList/InstrumentSelectionInfo.h \
    BreakdownPlotMarker.h \
    instrumentList/InstrumentList.h \
    StackedStockCharts.h \
    VolumePlot.h \
    VolumePlotCurve.h \
    VolumeYAxisScaleDraw.h \
    RegisterDialog.h \
    LicenseRegistration.h \
    SettingsHelper.h \
    instrumentList/InstrumentListTask.h \
    instrumentList/ReadQuotesFromFile.h \
    instrumentList/InstrumentListWorker.h \
    instrumentList/ScanPatternMatches.h \
    instrumentList/InstrumentListTypes.h

INCLUDEPATH += instrumentList

RESOURCES += icons.qrc


CONFIG(debug, debug|release) {
unix: LICENSEKEYLIBDIR = build-LicenseKeyLib-Desktop_Qt_5_3_GCC_64bit-Debug
macx: LICENSEKEYLIBDIR = build-LicenseKeyLib-Desktop_Qt_5_3_clang_64bit-Debug
macx: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug
win32: LICENSEKEYLIBDIR = build-LicenseKeyLib-Desktop_Qt_5_3_MinGW_32bit-Debug\debug
win32: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_MinGW_32bit-Debug\debug
} else {
unix: LICENSEKEYLIBDIR = build-LicenseKeyLib-Desktop_Qt_5_3_GCC_64bit-Release
macx: LICENSEKEYLIBDIR = build-LicenseKeyLib-Desktop_Qt_5_3_clang_64bit-Release
macx: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Release
win32: LICENSEKEYLIBDIR = build-LicenseKeyLib-Desktop_Qt_5_3_MinGW_32bit-Release\release
win32: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_MinGW_32bit-Release\release
DEFINES += QT_NO_DEBUG_OUTPUT
}

## PatternRecognitionLib is linked from the library built within the sub-module

LIBS += -L$$PWD/lib/PatternRecognitionLib/$$PATTERNRECOGLIBDIR -lPatternRecognitionLib
PRE_TARGETDEPS += $$PWD/lib/PatternRecognitionLib/$$PATTERNRECOGLIBDIR/libPatternRecognitionLib.a

LIBS += -L$$PWD/lib/LicenseKey/$$LICENSEKEYLIBDIR -lLicenseKeyLib
PRE_TARGETDEPS += $$PWD/lib/LicenseKey/$$LICENSEKEYLIBDIR/libLicenseKeyLib.a


## Link with pre-built version of Boost.
# IMPORTANT: To avoid link errors, the boost libraries must come *after* PatternRecognitionLib in the
# build sequence. gcc is especialy sensitive to this.
macx: DEFINES += BOOST_ALL_DYN_LINK
macx: DEFINES += BOOST_LOG_DYN_LINK
macx: INCLUDEPATH += /usr/local/boost156/include
macx: LIBS += -L/usr/local/boost156/lib -lboost_date_time-mt -lboost_log-mt -lboost_log_setup-mt -lboost_unit_test_framework-mt
macx: include ( /usr/local/qwt-6.1.0/features/qwt.prf )
win32: INCLUDEPATH += c:/boost_1_56_0
win32:LIBS += -L"C:/boost_1_56_0/stage/lib/" -lboost_date_time-mgw48-mt-1_56 -lboost_log-mgw48-mt-1_56 -lboost_log_setup-mgw48-mt-1_56
win32: include ( c:/qwt-6.1.1/qwt.prf )

INCLUDEPATH += $$PWD/lib/PatternRecognitionLib/src/chartSegment\
    $$PWD/lib/PatternRecognitionLib/src/chartSegmentList\
    $$PWD/lib/PatternRecognitionLib/src/date\
    $$PWD/lib/PatternRecognitionLib/src/math\
    $$PWD/lib/PatternRecognitionLib/src/patternMatch\
    $$PWD/lib/PatternRecognitionLib/src/patternMatchFilter\
    $$PWD/lib/PatternRecognitionLib/src/patternMatchValidator\
    $$PWD/lib/PatternRecognitionLib/src/patternScan\
    $$PWD/lib/PatternRecognitionLib/src/patternShape\
    $$PWD/lib/PatternRecognitionLib/src/quoteData\
    $$PWD/lib/LicenseKey/LicenseKeyLib

OTHER_FILES += \
    icons/chartpatternfinder.ico \
    ChartPatternFinder.rc \
    icons/cpf-loadquotes.png \
    icons/cpf-register.png \
    icons/cpf-reloadquotes.png \
    icons/cpf-upgrade.png \
    icons/cpf-help.png
