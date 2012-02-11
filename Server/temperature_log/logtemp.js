var net = require('net');
var orm = require('orm');
var crypto = require('crypto');

var key = new Buffer([
		0xEE, 0xCC, 0x3C, 0x4D,
		0x31, 0xF9, 0xB2, 0xDA,
		0xBE, 0xEF, 0x37, 0xA2,
		0x43, 0x42, 0x55, 0x76
	]).toString('binary');

var iv = '';


var db = orm.connect('postgresql://node_temp:report@localhost/Thermostat', function( success, db) {
		if (!success) {
			console.log('Could not connect the database!\n\t%d:%s', db.number, db.message);
			return;
		}
		var Temp = db.define("temperature", {
			"occurance":  { "type": "date" },
			"temperature": { "type": "float"}
		});
		Temp.sync();

		var server = net.createServer(function(c) {
			c.on('data', function (data) {
				if (data.length == 16) {
					data = new Buffer(data.toString('binary') + 'paddingbug' , 'binary');
					var decrypter = crypto.createDecipheriv('AES-128-ECB', key, iv);
					var result = decrypter.update(data); // first block is all we need
					var dataBuf = new Buffer(result);
					var tempSend = dataBuf.readUInt8(0) + (dataBuf.readUInt8(1) / 100);
					console.log('temperature: ' + tempSend);
					new Temp({ "occurance": new Date() , "temperature": tempSend }).save(function (err) {
						if (err) {
							console.log('Could not safe temp in db');
						}
					});
				}
				else {
					console.log('invalid message: ' + dataBuf.toString('hex') + ' size: '+ dataBuf.length);
				}
			});
		}).listen(5555);
	});
