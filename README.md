# Custom HTTP Web Server in C

A lightweight, educational HTTP/1.1 web server implementation in C using POSIX sockets.

![Server Screenshot](https://img.shields.io/badge/Language-C-blue)
![License](https://img.shields.io/badge/License-Educational-green)

## 🚀 Features

- ✅ HTTP/1.1 protocol implementation
- ✅ Serves static files (HTML, CSS, JavaScript, images, text)
- ✅ Automatic MIME type detection (9+ file types)
- ✅ Comprehensive error handling (404, 403, 500)
- ✅ Path traversal attack prevention
- ✅ Modular, well-organized code architecture
- ✅ Professional build system (Makefile)
- ✅ Single-threaded, blocking I/O (educational focus)

## 📁 Project Structure

```
http-server/
├── src/
│   ├── main.c              # Server initialization and main loop
│   ├── http_handler.c      # HTTP request/response handling
│   ├── http_handler.h
│   ├── file_server.c       # File operations and serving
│   ├── file_server.h
│   ├── mime_types.c        # MIME type detection
│   └── mime_types.h
├── www/                    # Web content directory
│   ├── index.html          # Homepage
│   ├── about.html          # About page
│   ├── styles.css          # Stylesheet
│   ├── app.js              # JavaScript
│   └── test.txt            # Plain text example
├── build/                  # Build artifacts (ignored by git)
├── Makefile                # Build automation
├── .gitignore              # Git ignore rules
└── README.md               # This file
```

## 🛠️ Requirements

- **OS:** Linux, Unix-like system, or WSL (Windows Subsystem for Linux)
- **Compiler:** GCC (GNU Compiler Collection)
- **Standard:** C99
- **Libraries:** POSIX sockets (included in Linux)

## 📦 Installation

### Clone the repository
```bash
git clone https://github.com/YOUR_USERNAME/http-server.git
cd http-server
```

### Compile
```bash
make
```

## 🚀 Running the Server

### Start the server
```bash
./httpserver
```

Or use the Makefile shortcut:
```bash
make run
```

The server will start on **port 8080**. Access it at:
```
http://localhost:8080
```

### Stop the server
Press `Ctrl+C` in the terminal.

## 🧪 Testing

### Browser Testing
Open your browser and navigate to:
- **Homepage:** `http://localhost:8080/`
- **About page:** `http://localhost:8080/about.html`
- **Plain text:** `http://localhost:8080/test.txt`
- **404 error:** `http://localhost:8080/missing.html`

### Command Line Testing
```bash
# Test homepage
curl http://localhost:8080/

# View HTTP headers
curl -i http://localhost:8080/

# Test specific files
curl http://localhost:8080/about.html
curl http://localhost:8080/test.txt

# Test 404 error
curl http://localhost:8080/nonexistent.html
```

## 📚 Technical Details

### Supported File Types

| Extension | MIME Type | Description |
|-----------|-----------|-------------|
| `.html`, `.htm` | `text/html` | HTML documents |
| `.css` | `text/css` | Stylesheets |
| `.js` | `application/javascript` | JavaScript |
| `.json` | `application/json` | JSON data |
| `.txt` | `text/plain` | Plain text |
| `.png` | `image/png` | PNG images |
| `.jpg`, `.jpeg` | `image/jpeg` | JPEG images |
| `.gif` | `image/gif` | GIF images |
| `.pdf` | `application/pdf` | PDF documents |

### HTTP Status Codes

- **200 OK** - Request successful
- **400 Bad Request** - Malformed request or unsupported method
- **403 Forbidden** - Path traversal attempt detected
- **404 Not Found** - Requested file doesn't exist
- **500 Internal Server Error** - Server-side error

### Architecture

The server implements a simple request-response cycle:

```
Client → [socket] → accept() → recv() → parse_request()
    ↓
load_file() → detect_mime_type() → send_response()
    ↓
close() → [loop back to accept()]
```

### Key Functions

#### `main.c`
- `main()` - Server initialization and main accept loop
- `shutdown_handler()` - Graceful shutdown on Ctrl+C

#### `http_handler.c`
- `parse_http_request()` - Parse HTTP request line
- `send_http_response()` - Send HTTP response with headers
- `send_error_page()` - Generate and send error pages
- `handle_client_connection()` - Main client request handler

#### `file_server.c`
- `load_file()` - Load file from disk with security checks
- `is_path_safe()` - Prevent directory traversal attacks
- `free_file_info()` - Clean up allocated memory

#### `mime_types.c`
- `detect_content_type()` - Determine MIME type from file extension

## 🔒 Security Features

- **Path traversal protection** - Blocks URLs containing `..`
- **Bounded string operations** - Prevents buffer overflows
- **Input validation** - Checks HTTP method and request format
- **Safe memory handling** - Proper allocation and deallocation
- **Error containment** - Client errors don't crash server

## ⚙️ Optimizations

- **SO_REUSEADDR** - Allows immediate port reuse after restart
- **Exact memory allocation** - Uses `stat()` to allocate precise amount
- **Single read operation** - Reads entire file at once
- **Bounded buffers** - Prevents memory exhaustion
- **Immediate resource cleanup** - Closes files/sockets immediately after use

## 🎓 Educational Value

This project demonstrates:

### Socket Programming
- `socket()` - Create communication endpoint
- `bind()` - Associate socket with address/port
- `listen()` - Mark socket as passive
- `accept()` - Wait for and accept client connections
- `recv()` / `send()` - Receive and send data
- `close()` - Close connections

### HTTP Protocol
- Request parsing (method, URI, version)
- Response generation (status line, headers, body)
- Content-Type negotiation
- Status code handling

### File I/O in C
- `fopen()` / `fclose()` - Open and close files
- `fread()` - Read file content
- `stat()` - Get file metadata
- Binary mode handling

### Memory Management
- `malloc()` / `free()` - Dynamic memory allocation
- Leak prevention strategies
- Buffer overflow protection

### System Programming
- Signal handling (SIGINT)
- Process lifecycle management
- Error handling patterns

## 🚧 Limitations

- **Single-threaded** - Handles one client at a time
- **No HTTPS** - Plain HTTP only (no encryption)
- **No persistent connections** - Each request opens/closes connection
- **Memory-based file serving** - Loads entire file into memory
- **No CGI support** - Static files only (no server-side execution)
- **No compression** - Files served without gzip/deflate

## 🔮 Future Improvements

- [ ] Multi-threading with `pthread`
- [ ] HTTPS support with OpenSSL
- [ ] HTTP/1.1 keep-alive connections
- [ ] Chunked transfer encoding for large files
- [ ] Configuration file support
- [ ] Access logging to file
- [ ] Virtual host support
- [ ] CGI/FastCGI support
- [ ] Compression (gzip)
- [ ] Range requests (partial content)

## 📖 How It Works

### Server Startup
1. Create socket with `socket(AF_INET, SOCK_STREAM, 0)`
2. Set `SO_REUSEADDR` option for quick restart
3. Bind to `0.0.0.0:8080` with `bind()`
4. Start listening with `listen()`
5. Register signal handler for Ctrl+C

### Request Handling
1. `accept()` blocks until client connects
2. `recv()` reads HTTP request
3. Parse request line to extract method and URI
4. Validate method is GET
5. Build file path and check security
6. Load file from disk
7. Detect MIME type
8. Send HTTP response headers
9. Send file content
10. Close connection
11. Loop back to accept

### Error Handling
- **Socket errors** → Print error and exit (fatal)
- **File not found** → Send 404 response (client error)
- **Path traversal** → Send 403 response (security)
- **Memory errors** → Send 500 response (server error)

## 🧪 Assignment Compliance

| Requirement | Points | Implementation |
|------------|--------|----------------|
| Server runs and serves same page | 50% | Infinite accept loop in `main.c` |
| Serves different files correctly | 20% | URI parsing + dynamic file loading |
| Error handling | 10% | All syscalls checked, HTTP error responses |
| Multiple file types | 10% | 9 MIME types in `mime_types.c` |
| Optimizations | 10% | SO_REUSEADDR, bounded buffers, efficient I/O |
| **Total** | **100%** | ✅ All requirements met |

## 🤝 Contributing

This is an educational project. Feel free to:
- Report bugs
- Suggest improvements
- Fork and experiment
- Use as learning material

## 📝 License

This project is for **educational purposes only**.

## 🙏 Acknowledgments

- POSIX Socket API documentation
- HTTP/1.1 RFC 2616 specification
- C99 ISO standard
- Linux man pages

## 📧 Contact

For questions or discussions about this project:
- GitHub: deatharoundthecorner(https://github.com/deatharoundthecorner)
- Email: danan20040520@gmail.com

---

**⭐ If you found this project helpful, please star it!**
