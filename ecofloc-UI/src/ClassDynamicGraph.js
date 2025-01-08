class DynamicGraph {
    constructor(nomGraphique, color) {
        // Layout configuration for the graph
        this.layout = {
            xaxis: {
                rangemode: 'tozero',
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: { color: 'white' }
            },
            yaxis: {
                rangemode: 'tozero',
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: { color: 'white' }
            },
            margin: { l: 30, r: 30, t: 10, b: 20 },
            paper_bgcolor: 'rgba(0,0,0,0)',
            plot_bgcolor: 'rgba(0,0,0,0)',
            dragmode: false
        };

        // Data initialization
        this.data = [{
            y: [],
            line: { color: color },
            fill: 'tozeroy'
        }];

        // Graph name and visibility
        this.nomGraphique = nomGraphique;
        this.showGraph = true;

        // Create the initial plot
        Plotly.newPlot(this.nomGraphique, this.data, this.layout, { responsive: true, displayModeBar: false });
    }

    // Updates the plot with a new value
    updatePlot(value) {
        this.data[0].y.push(value);
        Plotly.update(this.nomGraphique, { y: [this.data[0].y] });
    }

    // Shows or hides the graph
    show(isVisible) {
        const element = document.getElementById(this.nomGraphique);
        this.showGraph = isVisible;
        element.parentElement.style.display = isVisible ? "block" : "none";
    }

    // Re-render the graph to ensure it updates correctly
    refreshGraph() {
        Plotly.newPlot(this.nomGraphique, this.data, this.layout, { responsive: true, displayModeBar: false });
        this.show(this.showGraph);
    }
}
