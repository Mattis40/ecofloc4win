/*export var exec = require('node:child_process').exec;

exec('echo hello',
    function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
             console.log('exec error: ' + error);
        }
    });*/

const express = require('express');
const { exec } = require('child_process');
const cors = require('cors'); // For cross-origin requests from the front-end

const app = express();
const port = 3000;

// Middleware
app.use(cors());
app.use(express.json());

// API Endpoint to Execute Shell Command
app.post('/execute', (req, res) => {
    const { command } = req.body;

    if (!command) {
    return res.status(400).send({ error: 'No command provided' });
    }

    exec(command, (error, stdout, stderr) => {
    if (error) {
        return res.status(500).send({ error: error.message });
    }
    if (stderr) {
        return res.status(400).send({ stderr });
    }
    res.send({ stdout });
    });
});

// Start Server
app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});

