// Data samples are stored here
var dataSet = [];

document.addEventListener( 'DOMContentLoaded', function () {
	// Run this once after the DOM is loaded
	if (!!window.EventSource) {
		// Good example on using SSE
		// http://www.html5rocks.com/en/tutorials/eventsource/basics/
		
		var source = new EventSource('data');
		source.addEventListener('message', function(e) {
			// e.data is the SSE data, which is a two-character hexadecimal string representing a value
			// console.log("msg=" + e.data);
                                
            for(var ii = 0; ii < e.data.length; ii += 2) {
                handleData(parseInt('0x' + e.data.substr(ii, 2)));
            }
		}, false);
	}
	else {
		console.log('sse not supported');
	}
}, false );


function handleData(data) {
    // console.log(data);

    // Display data stream onto the page
    var dataStream = document.getElementById("dataStream");
    dataStream.innerHTML = `${data}`;

    var canvas = document.getElementById("canvas");
    var ctx = canvas.getContext("2d");
    
    
	
	// Add to the data set, remove from the left if it gets wider than the canvas
	dataSet.push(data);
	if (dataSet.length > (canvas.width - 1)) {
		dataSet.shift();
	}
	
	// Erase
	ctx.fillStyle = "#ffffff";
	ctx.fillRect(0, 0, canvas.width, canvas.height);	
	
	// Draw samples
	ctx.fillStyle = "#000000";
	
	for(var ii = 0; ii < dataSet.length; ii++) {
		// The canvas coordinate space increases going down the page, but the graph
		// makes more sense flipped the other way so subtract the value from the 
		// maximum value
		var yy = 255 - dataSet[ii];

		ctx.fillRect(ii, yy, 1, 1);
    }

    var data = {
        labels: ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'],
        series: [
            [dataSet[dataSet.length - 10], dataSet[dataSet.length - 9], dataSet[dataSet.length - 8], dataSet[dataSet.length - 7], dataSet[dataSet.length - 6],
            dataSet[dataSet.length - 5], dataSet[dataSet.length - 4], dataSet[dataSet.length - 3], dataSet[dataSet.length - 2], dataSet[dataSet.length - 1]],
        ]
    };
      
    // As options we currently only set a static size of 300x200 px. We can also omit this and use aspect ratio containers
    // as you saw in the previous example
    var options = {
        fullWidth: true,
        height: 255,
        high: 255,
        low: 0,
    };

    new Chartist.Line('.ct-chart', data, options);
}

