class DynamicGraph {
    constructor(nomGraphique) {
        this.layout = {
            xaxis: {
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: { color: 'white' }
            },
            yaxis: {
                gridcolor: 'rgba(255,255,255,0.2)',
                tickfont: { color: 'white' }
            },
            margin: { l: 30, r: 30, t: 10, b: 20 },
            paper_bgcolor: 'rgba(0,0,0,0)',
            plot_bgcolor: 'rgba(0,0,0,0)',
            dragmode: false,
            showlegend: false
        };

        this.data = {};
        this.traceIndices = {};
        this.nomGraphique = nomGraphique;

        Plotly.newPlot(this.nomGraphique, [], this.layout, { responsive: true, displayModeBar: false });
    }

    updatePlot(PID, value, color) {
        if (!this.data[PID]) {
            if (PID !== "TOTAL") {
                this.data[PID] = {
                    y: [],
                    line: { color:color },
                    fill: 'none',
                    name: `PID ${PID}`,
                };
            }
            else{
                this.data["TOTAL"] = {
                    y: [],
                    line: { color: "#10b981" },
                    fill: 'tozeroy',
                };
            }
            
            Plotly.addTraces(this.nomGraphique, this.data[PID]);
            this.traceIndices[PID] = Object.keys(this.traceIndices).length; // Associe un index à ce PID
        }

        this.data[PID].y.push(value);

        const index = this.traceIndices[PID];
        if (index === undefined) {
            console.error(`Error: PID ${PID} not found in trace indices`);
            return;
        }

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
