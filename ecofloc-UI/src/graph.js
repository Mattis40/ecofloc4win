class DynamicGraph {
    constructor(graphName, color) {
        // Define the layout for the plot
        this.layout = {
            xaxis: {
                rangemode: 'tozero', 
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: {
                    color: 'white'               // Color of numbers on the X axis
                  } 
            },
            yaxis: {
                rangemode: 'tozero',
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: {
                    color: 'white'               // Color of numbers on the X axis
                }
            },
            margin: {
                l: 30,  // Left margin
                r: 30,  // Right margin
                t: 10,  // Up margin
                b: 20   // Down margin
            },
            paper_bgcolor: 'rgba(0,0,0,0)',  // Transparent outer background
            plot_bgcolor: 'rgba(0,0,0,0)',   // Transparent chart background
            dragmode: false
        };
        // Define the initial data with an empty array for 'y'
        this.data = [{
            y: [], // Starting values
            line: {color: color}, // Green line color
            fill: 'tozeroy',
        }];

        // Set the graph name
        this.graphName = graphName;
        this.showGraph = true
        // Create the initial plot
        Plotly.newPlot(this.graphName, this.data, this.layout, { responsive: true, displayModeBar: false });
    }

    // Method to update the plot by pushing new values to 'y'
    updatePlot(value) {
        // Get the current data from the plot
        var currentY = this.data[0].y;

        // Push the new value to the 'y' array
        currentY.push(value);

        // Update the plot with the new data
        Plotly.update(this.graphName, {
            y: [currentY]
        });
    }
    show(value) {
        const element = document.getElementById(this.graphName);
        this.showGraph = value;
        if (value) {
            element.parentElement.style.display = "block"; // Show the element
        } else {
            element.parentElement.style.display = "none"; // Hide the element
        }
    }

    updateElements() {
        const currentElement = document.getElementById(this.graphName);
        const graphData = this.data;
        const layout = this.layout;
        Plotly.newPlot(this.graphName, graphData, layout, { responsive: true, displayModeBar: false });
        this.show(this.showGraph);
    }
}
