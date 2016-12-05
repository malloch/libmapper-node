var nbind = require('nbind');

var lib = nbind.init(__dirname).lib;

//module.exports = binding.lib;

dev = new lib.Device('foo');

for (i = 0; i < 200; i++)
    dev.poll(100);

dev.logout()
