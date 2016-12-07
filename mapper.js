var nbind = require('nbind');

var lib = nbind.init(__dirname).lib;

//module.exports = binding.lib;

dev = new lib.Device('foo');
insig = dev.add_input_signal('in', 1, 'i', function() { console.log('js update handler');});
outsig = dev.add_output_signal('out', 1, 'i');

while (!dev.ready()) {
    console.log("waiting for device initialization");
    dev.poll(100);
}

map = new lib.Map(outsig, insig).push();

for (i = 0; i < 200; i++) {
    outsig.update(parseInt(i));
    dev.poll(100);
}


