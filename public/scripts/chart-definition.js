// Create the charts when the web page loads
window.addEventListener('load', onload);

function onload(event){
  chartV = createValueChart();
}

// Create Temperature Chart
function createValueChart() 
{
    var chart = new Highcharts.Chart({
      chart:{ 
        renderTo:'chart-values',
        type: 'line' 
      },
      series: [
        {
          name: 'Values'
        }
      ],
      title: { 
        text: undefined
      },
      plotOptions: {
        line: { 
          animation: false,
          dataLabels: { 
            enabled: false 
          }
        },
        series: { 
          color: '#E10606' 
        }
      },
      xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
      },
      yAxis: {
        title: { 
          text: 'Values' 
        }
      },
      credits: { 
        enabled: false 
      }
    });
    return chart;
}