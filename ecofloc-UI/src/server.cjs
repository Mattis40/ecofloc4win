const express = require('express');
const { exec } = require('child_process');
const cors = require('cors'); // To enable queries from your front-end

const app = express();
const port = 3000;

let processRunning = false; // Indicates whether the process is running
let configuratorRunning = false; // Indicates whether the configurator is running

// Middleware
app.use(cors());
app.use(express.json());

// API to launch the executable
app.post('/execute', (req, res) => {
    const exePath = `"${__dirname}\\testjson.exe"`; // Absolute path to executable

    if (processRunning) {
        return res.status(400).json({ success: false, message: 'A process is already underway.' });
    }

    try {
        // Run the executable
        const process = exec(exePath, (error, stdout, stderr) => {
            processRunning = false; // Reset after execution
            if (error) {
                console.error(`Runtime error: ${error.message}`);
                return; // No `res.json` here because the response has already been sent
            }

            if (stderr) {
                console.warn(`Stderr: ${stderr}`);
                return; // Same, manage without additional response
            }

            console.log(`Stdout: ${stdout}`);
        });

        processRunning = true; // Indicates that the process has started
        console.log('Process started.');
        return res.json({ success: true, message: 'Process started.' });
    } catch (error) {
        console.error(`Unexpected error: ${error.message}`);
        processRunning = false; // Ensure the state is reset
        return res.status(500).json({ success: false, message: 'Error starting process.' });
    }
});

// API to launch the configurator
app.post('/configurator', (req, res) => {
    const exePath = `"${__dirname}\\EcoflocConfigurator.exe"`; // Absolute path to the configurator

    if (configuratorRunning) {
        return res.status(400).json({ success: false, message: 'Configurator is already underway.' });
    }

    try {
        const process = exec(exePath, (error, stdout, stderr) => {
            configuratorRunning = false; // Reset after execution
            if (error) {
                console.error(`Runtime error: ${error.message}`);
                return;
            }
            if (stderr) {
                console.warn(`Stderr: ${stderr}`);
                return;
            }
            console.log(`Stdout: ${stdout}`);
        });

        configuratorRunning = true;
        console.log('Configurator started.');
        return res.json({ success: true, message: 'Configurator started.' });
    } catch (error) {
        console.error(`Unexpected error: ${error.message}`);
        configuratorRunning = false;
        return res.status(500).json({ success: false, message: 'Error starting configurator.' });
    }
});


// API to stop the executable
app.post('/stop', (req, res) => {
    const exeName = 'testjson.exe'; // Name of process to kill

    if (!processRunning) {
        return res.status(400).json({ success: false, message: 'No processes running.' });
    }

    try {
        // Using taskkill to kill process by name
        exec(`taskkill /IM ${exeName} /F`, (error, stdout, stderr) => {
            if (error) {
                console.error(`Error during shutdown: ${error.message}`);
                return res.status(500).json({ success: false, message: `Error: ${error.message}` });
            }

            if (stderr) {
                console.warn(`Stderr: ${stderr}`);
                // Optional: send a warning
            }

            console.log(`Process stopped: ${stdout}`);
            processRunning = false; // Reset after shutdown
            return res.json({ success: true, message: 'Process stopped.' });
        });
    } catch (error) {
        console.error(`Unexpected error: ${error.message}`);
        processRunning = false; // Ensure the state is reset
        return res.status(500).json({ success: false, message: 'Error stopping process.' });
    }
});

// Start the server
app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});
