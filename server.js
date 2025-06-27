// A simple server to bridge the UI and the C++ backend.
const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const app = express();
const port = 3000;

// Middleware to parse JSON bodies and serve static files (like index.html)
app.use(express.json());
app.use(express.static(path.join(__dirname))); 

// ==============================================================================
// IMPORTANT: Replace './jit_compiler' with the actual path to your C++ executable.
// On Windows, it might be './jit_compiler.exe'. On Linux/macOS, it's often './jit_compiler'.
const JIT_EXECUTABLE = './jit_compiler';
// ==============================================================================


// This is the API endpoint that the frontend will call.
app.post('/run', (req, res) => {
    const script = req.body.script;
    if (!script) {
        return res.status(400).json({ error: 'No script provided.' });
    }

    // FIX: Pre-process the script to remove comments and carriage returns (\r).
    // This prevents issues where a variable might be stored as "n\r" and loaded as "n".
    const scriptWithoutComments = script.replace(/\r/g, "").split('\n').map(line => line.split(';')[0]).join('\n');

    // Sanitize and escape the script to be passed as a single, safe command-line argument.
    // This prevents command injection issues.
    const command = `${JIT_EXECUTABLE} "${scriptWithoutComments.replace(/"/g, '\\"')}"`;

    // Execute the C++ compiled program as a child process.
    exec(command, (error, stdout, stderr) => {
        // If the C++ program returned an error code...
        if (error) {
            // 'stderr' is where the C++ program prints its JSON errors.
            // We try to parse it and send it back to the frontend.
            try {
                // The C++ program should have printed a JSON error message.
                return res.status(500).json(JSON.parse(stderr));
            } catch (e) {
                // If parsing fails, send the raw error text.
                return res.status(500).json({ error: stderr || "An unknown execution error occurred." });
            }
        }
        
        // If execution was successful, 'stdout' contains the JSON output.
        try {
            res.json(JSON.parse(stdout));
        } catch(e) {
            // This catches errors if the C++ output isn't valid JSON.
            res.status(500).json({ error: 'Failed to parse C++ output. Output was: ' + stdout });
        }
    });
});

// Start the server.
app.listen(port, () => {
    console.log(`Server listening at http://localhost:${port}`);
    console.log(`Serving UI from: ${path.join(__dirname, 'index.html')}`);
});
