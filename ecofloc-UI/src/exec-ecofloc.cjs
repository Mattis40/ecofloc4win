const express = require('express');
const { exec } = require('child_process');
const cors = require('cors'); // For cross-origin queries from a front-end

const app = express();
const port = 3000;

// Middleware
app.use(cors());
app.use(express.json());

// Endpoint API to run an .exe file
app.post('/execute', (req, res) => {
    const exePath = 'echo hello'; // Replace with the absolute path of your .exe file

    exec(`"${exePath}"`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Runtime error: ${error.message}`);
            return res.status(500).send({ error: error.message });
        }

        if (stderr) {
            console.warn(`Stderr: ${stderr}`);
            return res.status(400).send({ stderr });
        }

        console.log(`Stdout: ${stdout}`);
        res.send({ stdout });
    });
});

// Start the server
app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});
