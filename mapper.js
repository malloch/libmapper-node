var nbind = require('nbind');

var lib = nbind.init(__dirname).lib;

//module.exports = binding.lib;

net = new lib.Network();
console.log('interface: '+net.interface);

//dev = new lib.Device('foo');
//insig = dev.add_input_signal('in', 1, 'i',
//                             function(sig, value) {
//                                 console.log('js update handler');
//                                 console.log('sig '+sig.name()+' = '+value);
//                             });
//outsig = dev.add_output_signal('out', 1, 'i');
//
//while (!dev.ready()) {
//    console.log("waiting for device initialization");
//    dev.poll(100);
//}
//
//map = new lib.Map(outsig, insig).push();
//
//for (i = 0; i < 200; i++) {
//    outsig.update(parseInt(i));
//    dev.poll(100);
//    if (i == 100)
//        insig.set_callback(function(sig, value) {
//                               console.log('js update handler 2');
//                               console.log('sig '+sig.name()+' = '+value);
//                           })
//}

//db = new lib.Database();
//
//db.add_device_callback(function(dev, action){
//                           console.log("js db.device_cb");
//                           console.log("  dev: "+dev.name()+", action: "+action);
//                       })
//db.add_signal_callback(function(sig, action){
//                           console.log("js db.signal_cb");
//                           console.log("  sig: "+sig.name()+", action: "+action);
//                       });
//db.add_link_callback(function(link, action){
//                         console.log("js db.link_cb");
//                         console.log("  link: "+link.device(0).name()+" <-> "+link.device(1).name()+", action: "+action);
//                     });
//db.add_map_callback(function(map, action){
//                        console.log("js db.map_cb");
//                        console.log("  source: "+map.source(0).signal());
//                        console.log("  destination: "+map.destination().signal());
//                        console.log("  action: "+action);
//                     });

//for (i = 0; i < 200; i++) {
//    db.poll(100);
//}

