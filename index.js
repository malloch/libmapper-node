var nbind = require('nbind');

var lib = nbind.init(__dirname).lib;

//module.exports = binding.lib;

net = new lib.Network();
console.log('interface: '+net.interface);

dev = new lib.Device('foo');
insig = dev.add_input_signal('in', 1, 'i',
                             function(sig, value) {
                                 console.log('js update handler');
                                 console.log('sig \''+sig.name+'\' = '+value);
                             });
outsig = dev.add_output_signal('out', 1, 'i');

while (!dev.ready) {
    console.log("waiting for device initialization");
    dev.poll(100);
}

map = new lib.Map(outsig, insig).set_expression("y=x*10").push();

for (i = 0; i < 20; i++) {
    outsig.update(parseInt(i));
    dev.poll(10);
    if (i == 100)
        insig.set_callback(function(sig, value) {
                               console.log('js update handler 2');
                               console.log('sig \''+sig.name+'\' = '+value);
                           })
}

db = new lib.Database();

db.add_device_callback(function(dev, event) {
                           console.log("js db.device_cb");
                           console.log("  dev: \'"+dev.name+"\', event: "+event);
                       });

db.add_signal_callback(function(sig, event) {
                           console.log("js db.signal_cb");
                           console.log("  sig: \'"+sig.name+"\', action: "+event);
                       });

db.add_link_callback(function(link, event){
                         console.log("js db.link_cb");
                         console.log("  link: \'"+link.device(0).name+"\' <-> \'"+link.device(1).name+"\', action: "+event);
                     });

db.add_map_callback(function(map, event){
                        console.log("js db.map_cb");
                        console.log("  source: \'"+map.source(0).signal().name+"\'");
                        console.log("  destination: \'"+map.destination(0).signal().name+"\'");
                        console.log("  action: "+event);
                     });


//console.log("done adding callbacks");

for (i = 0; i < 100; i++) {
    db.poll(100);
    dev.poll(10);
}
//
//function* foo(arg) {
//    console.log("  devices()");
//    var devs = arg;
//    var got_next = arg.check();
//    while(got_next) {
//        console.log("  iterating!!!");
//        var dev = new lib.Device(devs.deref());
//        got_next = devs.next();
//        yield dev;
//    }
//}
//
//console.log("db.devices()");
//var devs = foo(db.devices());
//
//for(value of devs){
//    console.log(value);
//    for (i in value)
//        console.log("  ", i);
//    console.log("  try to retrieve name...");
//    console.log("  ", value['name']);
//}
//
//
//var objectType = {
//    none:               0x00,
//    devices:            0x01,
//    inputSignals:       0x02,
//    outputSignals:      0x04,
//    signals:            0x06,
//    incomingLinks:  	0x10,
//    outgoingLinks:      0x20,
//    links:              0x30,
//    incomingMaps:   	0x40,
//    outgoingMaps:       0x80,
//    maps:           	0xC0,
//    all:                0xFF
//};
//
//var operator = {
//    doesNotExist:       0x00,
//    equal:              0x01,
//    exists:             0x02,
//    greaterThan:        0x03,
//    greaterThanOrEqual: 0x04,
//    lessThan:           0x05,
//    lessThanOrEqual:    0x06,
//    notEqual:           0x07
//};
//
//var boundaryAction = {
//    undefined:          0x00,
//    none:           	0x01,
//    mute:               0x02,
//    clamp:              0x03,
//    fold:               0x04,
//    wrap:               0x05
//};
//
//var mode = {
//    undefined:          0x00,
//    raw:                0x01,
//    linear:             0x02,
//    expression:         0x03
//};
//
//var location = {
//    undefined:          0x00,
//    source:             0x01,
//    destination:        0x02
//};
//
//var direction = {
//    any:                0x00,
//    incoming:       	0x01,
//    outgoing:       	0x02,
//    both:               0x03
//};
//
//var instanceEvent = {
//    new:                0x01,
//    upstreamRelease:    0x02,
//    downstreamRelease:  0x04,
//    overflow:           0x08,
//    all:                0x0F
//};
//
//var instanceStealingType = {
//    none:               0x00,
//    stealOldest:        0x01,
//    stealNewest:        0x02
//};
//
//var recordEvent = {
//    added:              0x00,
//    modified:       	0x01,
//    removed:            0x02,
//    expired:            0x03
//};
//
//module.exports.objectType = objectType;
//module.exports.operator = operator;
//module.exports.boundaryAction = boundaryAction;
//module.exports.mode = mode;
//module.exports.location = location;
//module.exports.direction = direction;
//module.exports.instanceEvent = instanceEvent;
//module.exports.instanceStealingType = instanceStealingType;
//module.exports.recordEvent = recordEvent;
//
//
//
//
