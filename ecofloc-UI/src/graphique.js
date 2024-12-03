class DynamicGraph {
    constructor(nomGraphique, color) {
        // Define the layout for the plot
        this.layout = {
            xaxis: {
                rangemode: 'tozero', 
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: {
                    color: 'white'               // Couleur des nombres sur l'axe X
                  } 
            },
            yaxis: {
                rangemode: 'tozero',
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: {
                    color: 'white'               // Couleur des nombres sur l'axe X
                }
            },
            margin: {
                l: 30,  // Marge gauche
                r: 30,  // Marge droite
                t: 10,  // Marge en haut
                b: 20   // Marge en bas
            },
            paper_bgcolor: 'rgba(0,0,0,0)',  // Fond extérieur transparent
            plot_bgcolor: 'rgba(0,0,0,0)',   // Fond du graphique transparent
            dragmode: false
        };
        // Define the initial data with an empty array for 'y'
        this.data = [{
            y: [], // Starting values
            line: {color: color}, // Couleur de la ligne verte
            fill: 'tozeroy',
        }];

        // Set the graph name
        this.nomGraphique = nomGraphique;
        this.showGraph = true
        // Create the initial plot
        Plotly.newPlot(this.nomGraphique, this.data, this.layout, { responsive: true, displayModeBar: false });
    }

    // Method to update the plot by pushing new values to 'y'
    updatePlot(value) {
        // Get the current data from the plot
        var currentY = this.data[0].y;

        // Push the new value to the 'y' array
        currentY.push(value);

        // Update the plot with the new data
        Plotly.update(this.nomGraphique, {
            y: [currentY]
        });
    }
    show(value) {
        const element = document.getElementById(this.nomGraphique);
        this.showGraph = value;
        if (value) {
            element.parentElement.style.display = "block"; // Show the element
        } else {
            element.parentElement.style.display = "none"; // Hide the element
        }
    }

    mettreAJourElement() {
        const currentElement = document.getElementById(this.nomGraphique);
        const graphData = this.data;
        const layout = this.layout;
        Plotly.newPlot(this.nomGraphique, graphData, layout, { responsive: true, displayModeBar: false });
        this.show(this.showGraph);
    }
}
