/***** SERVER CODE *****/
// var net = require('net');

// var server = net.createServer(function(socket) {
// 	socket.write('Echo server\r\n');
// 	socket.pipe(socket);
// });

// server.listen(1337, '127.0.0.1');

/***** CLIENT CODE *****/
var net = require('net');
var client = new net.Socket();

client.connect(8081, '192.168.229.135', function() {
    console.log('Connected'); 
});

setInterval(function(){ 
    client.write("Message: " + String(Math.floor((Math.random() * 255) + 1)));  
    // client.write("Hello");  
}, 100);

client.on('data', function(data) {
	console.log('Received: ' + data);
	client.destroy(); // kill client after server's response
});

client.on('close', function() {
	console.log('Connection closed');
});