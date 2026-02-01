console.log('JavaScript loaded successfully from custom HTTP server');
console.log('Server implementation: C with POSIX sockets');
console.log('Protocol: HTTP/1.1');

function updateTimestamp() {
    const now = new Date();
    const options = {
        weekday: 'long',
        year: 'numeric',
        month: 'long',
        day: 'numeric',
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit',
        hour12: true
    };
    
    const timestampElement = document.getElementById('timestamp');
    if (timestampElement) {
        timestampElement.textContent = 'Current time: ' + now.toLocaleString('en-US', options);
    }
}

document.addEventListener('DOMContentLoaded', function() {
    console.log('📄 DOM loaded - all resources fetched successfully');
    updateTimestamp();
    setInterval(updateTimestamp, 1000);
});
