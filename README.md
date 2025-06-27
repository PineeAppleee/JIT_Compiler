⚙️ JIT Compiler Interactive UI
This project provides a simple, browser-based interface to interact with a Just-In-Time (JIT) C++ compiler backend. Users can input stack-based instructions, execute them via a server, and see the resulting output, stack, and memory visuals.

📁 Project Structure
.
├── index.html # Frontend UI (HTML + TailwindCSS + JavaScript)
├── Makefile # Automates backend build & run process
├── README.md # Project documentation

🌟 Features
🧠 In-browser code editor for stack-based scripts

🛠 "Run on Server" button to execute code via backend

📤 Output display showing final result

🧮 Visualizations for:

Stack

Variables

Array Stack

Call Stack

🧾 Example Input
PUSH 100
PUSH 20
SUB
PUSH 5
MUL
PRINT

📤 Expected Output:
400

🚀 How to Run This Project
1. 🔨 Build the Backend
If you're using a C++ backend:
make

This will compile your backend code if the Makefile is properly set up.

Make sure your backend listens for POST requests at /run and returns output as JSON.

2. 🌐 Serve the Frontend
To open the index.html in a browser:

Option A: Using Python
python3 -m http.server 8080

Then open your browser and visit:
http://localhost:8080/index.html

📡 Backend API Expected Behavior
The frontend sends a POST request to /run with a JSON body:

{
"script": "PUSH 100\nPUSH 20\nSUB\nPUSH 5\nMUL\nPRINT"
}

The backend should return a response like:

{
"output": "400",
"stack": [400],
"variables": {},
"array_stack": [],
"call_stack": []
}

🧪 Tech Used
Frontend: HTML, TailwindCSS, JavaScript
Backend: C++ (assumed JIT compiler)
Server: Localhost (can use Python or Node)

📝 Notes
Empty stacks and data structures are represented with "empty".

All code is executed via the server, not locally in the browser.

You can modify the input script and reset the editor with the "Reset" button.

🧑‍💻 Author
Built with 💻 and ❤️ by Kanishk Joshi

📄 License
MIT License. Free to use, modify, and share.
