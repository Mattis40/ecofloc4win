/*class DynamicGraph {
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
}*/
class DynamicGraph {
    constructor(nomGraphique, defaultColor) {
        // Configuration du layout pour le graphique
        this.layout = {
            xaxis: {
                rangemode: 'scatter',
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: { color: 'white' }
            },
            yaxis: {
                rangemode: 'scatter',
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: { color: 'white' }
            },
            margin: { l: 30, r: 30, t: 10, b: 20 },
            paper_bgcolor: 'rgba(0,0,0,0)',
            plot_bgcolor: 'rgba(0,0,0,0)',
            dragmode: false,
            showlegend: false
        };

        this.data = {}; // Stockage des séries par PID
        this.traceIndices = {}; // Lien entre PID et index des traces
        this.nomGraphique = nomGraphique;
        this.defaultColor = defaultColor;

        // Initialisation du graphique
        Plotly.newPlot(this.nomGraphique, [], this.layout, { responsive: true, displayModeBar: false });
    }

    updatePlot(PID, value) {
        // Vérifie si le PID existe déjà
        if (!this.data[PID]) {
            console.log(`Initialisation de la série pour PID: ${PID}`);
            const color = this.getRandomColor();
            this.data[PID] = {
                y: [],
                line: { color:color },
                fill: 'scatter',
                name: `PID ${PID}`
            };

            // Ajoute une nouvelle trace pour ce PID
            Plotly.addTraces(this.nomGraphique, this.data[PID]);
            this.traceIndices[PID] = Object.keys(this.traceIndices).length; // Associe un index à ce PID
        }

        // Ajoute la valeur à la série correspondante
        this.data[PID].y.push(value);

        // Vérifie si un index existe pour ce PID
        const index = this.traceIndices[PID];
        if (index === undefined) {
            console.error(`Erreur : PID ${PID} non trouvé dans les indices des traces`);
            return;
        }

        // Met à jour la trace correspondante
        console.log(`Mise à jour de la trace pour PID ${PID} à l'index ${index}`);
        Plotly.update(this.nomGraphique, { y: [this.data[PID].y] }, {}, [index]);
    }

    refreshGraph() {
        const traces = Object.values(this.data);
        Plotly.newPlot(this.nomGraphique, traces, this.layout, { responsive: true, displayModeBar: false });
    }

    getRandomColor() {
        const r = Math.floor(Math.random() * 200)+55;
        const g = Math.floor(Math.random() * 200)+55;
        const b = Math.floor(Math.random() * 200)+55;
        return `rgb(${r}, ${g}, ${b})`;
    }

}
