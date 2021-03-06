
#include <qwt_legend.h>
#include <qwt_plot_zoneitem.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_panner.h>
#include <qwt_legend_label.h>
#include <qwt_date.h>
#include <qwt_date_scale_engine.h>
#include <qwt_date_scale_draw.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_plot_barchart.h>
#include <qwt_plot_legenditem.h>
#include <assert.h>

#include "PriceAndPatternPlot.h"

#include "StockChartDateScaleDraw.h"
#include "StockChartPlotZoomer.h"
#include "StockChartPlotCurve.h"
#include "BreakoutPlotMarker.h"
#include "BreakdownPlotMarker.h"
#include "PatternPlotCurve.h"
#include "QDateHelper.h"

#include "PeriodValSegment.h"
#include "DoubleBottomScanner.h"
#include "PatternShapeGenerator.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "SymetricTriangleScanner.h"
#include <sstream>
#include <qwt_scale_engine.h>

PriceAndPatternPlot::PriceAndPatternPlot( QWidget *parent ):
    QwtPlot( parent )
{
    setTitle( "" );

    QwtLinearScaleEngine *scaleEngine = new QwtLinearScaleEngine(10);

    setAxisScaleEngine( QwtPlot::xBottom, scaleEngine );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );

    // The following is needed to properly adjust the RHS of the X axis. Otherwise,
    // there is space on the RHS.
    axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating,true);


    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    // TODO: Check the memory ownership/leak for the following allocation
    plotZoomer_ = new StockChartPlotZoomer( canvas() );

    QwtPlotPanner *panner = new QwtPlotPanner( canvas() );
    panner->setMouseButton( Qt::MidButton );

    // Attach a dotted-line grid to the plot.
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setItemAttribute(grid->Legend, false);
    grid->setPen(QColor(Qt::lightGray), 0.0, Qt::PenStyle::DotLine);
    grid->attach(this);

    // Override the size policy. Otherwise, the plot may not scale to
    // the desired dimensions from the grid layout.
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMinimumSize(0,0);

    // Attach a legend internal to the plot
    QwtPlotLegendItem *legend = new QwtPlotLegendItem();
    legend->setAlignment(Qt::Alignment(Qt::AlignTop | Qt::AlignLeft));
    legend->attach(this);

    // Set the background color to white
    setAutoFillBackground( true );
    setPalette( Qt::white );
    setCanvasBackground( Qt::white );

}


void PriceAndPatternPlot::clearPatternPlots()
{
    // Detach and delete any existing plot curves
    this->detachItems(QwtPlotItem::Rtti_PlotCurve,true);
    this->detachItems(QwtPlotItem::Rtti_PlotMarker,true);

}

void PriceAndPatternPlot::populateOnePatternShape(const PatternMatchPtr &patternMatch)
{
    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*patternMatch);
    PatternShapePointVectorVectorPtr curveShapes = patternShape->curveShapes();


    // Re-populate with the pattern for the given patternMatch
    for(PatternShapePointVectorVector::iterator curveShapeIter = curveShapes->begin();
        curveShapeIter != curveShapes->end(); curveShapeIter++)
    {
        bool doCurveFit = true;
        QwtPlotCurve *patternMatchPlot = new PatternPlotCurve(*curveShapeIter,doCurveFit);
        patternMatchPlot->attach(this);
    }

    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();
    for(PatternShapePointVectorVector::iterator lineShapeIter = lineShapes->begin();
        lineShapeIter != lineShapes->end(); lineShapeIter++)
    {
        bool doCurveFit = false;
        QwtPlotCurve *patternMatchPlot = new PatternPlotCurve(*lineShapeIter,doCurveFit);
        patternMatchPlot->attach(this);
    }


    if(patternMatch->breakoutInfo)
    {
        BreakoutPlotMarker *breakoutPlotMarker = new BreakoutPlotMarker(
                    patternMatch->breakoutInfo->pseudoXVal(),patternMatch->breakoutInfo->breakoutPrice());
        breakoutPlotMarker->attach(this);
    }
    else if(patternMatch->breakdownInfo)
    {
        BreakdownPlotMarker *breakdownPlotMarker = new BreakdownPlotMarker(
                    patternMatch->breakdownInfo->pseudoXVal(),patternMatch->breakdownInfo->breakoutPrice());
        breakdownPlotMarker->attach(this);
    }

    replot();

}

void PriceAndPatternPlot::populatePatternMatchesShapes(const PatternMatchListPtr &patternMatches)
{
    clearPatternPlots();

    for(PatternMatchList::iterator matchesIter = patternMatches->begin();
        matchesIter != patternMatches->end(); matchesIter++)
    {
        populateOnePatternShape(*matchesIter);
    }
}



void PriceAndPatternPlot::populateChartData(const InstrumentSelectionInfoPtr &instrSelInfo)
{

    clearPatternPlots();
    this->detachItems(QwtPlotItem::Rtti_PlotTradingCurve,true);

    setTitle(instrSelInfo->instrumentName());

    QwtDateScaleDraw *scaleDraw = new StockChartDateScaleDraw( Qt::UTC,instrSelInfo->chartData() );
    setAxisScaleDraw( QwtPlot::xBottom, scaleDraw );

    StockChartPlotCurve *chartDataCurve = new StockChartPlotCurve(instrSelInfo->chartData());
    chartDataCurve->attach( this );
    showItem( chartDataCurve, true );

    // Rescale the plot based upon the boundaries of the current chart data
    setAxisAutoScale( QwtPlot::yLeft );
    setAxisAutoScale( QwtPlot::xBottom);

    // Update the chart data for the plot zoomer, so it can show a curser with appropriate data.
    plotZoomer_->setChartData(instrSelInfo->chartData());

    replot();

    // The following has the effect of freezing the maximum zoom coordinates to the
    // initial scale of the chart. This needs to happen after replot(). The scale
    // for zooming needs to be reset whenever the chart data changes.
    plotZoomer_->setZoomBase(false);

}

void PriceAndPatternPlot::setMode( int style )
{
    QwtPlotTradingCurve::SymbolStyle symbolStyle =
        static_cast<QwtPlotTradingCurve::SymbolStyle>( style );

    QwtPlotItemList curves = itemList( QwtPlotItem::Rtti_PlotTradingCurve );
    for ( int i = 0; i < curves.size(); i++ )
    {
        QwtPlotTradingCurve *curve =
            static_cast<QwtPlotTradingCurve *>( curves[i] );
        curve->setSymbolStyle( symbolStyle );
    }

    replot();
}

void PriceAndPatternPlot::showItem( QwtPlotItem *item, bool on )
{
    item->setVisible( on );
    replot();
}

void PriceAndPatternPlot::exportPlot()
{
    QwtPlotRenderer renderer;
    renderer.exportTo( this, "stockchart.pdf" );
}
