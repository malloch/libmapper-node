#include <mapper/mapper_cpp.h>
#include "nbind/api.h"  // for nbind::cbFunction type
#include "nbind/nbind.h"

class Signal : public mapper::Signal
{
public:
    Signal(mapper::Signal sig) : mapper::Signal(sig) {}

//    using mapper::Signal::set_callback;
    Signal& set_callback(nbind::cbFunction& cb)
    {
        void *callback = user_data();
        if (callback)
            delete((nbind::cbFunction*)callback);
        callback = (nbind::cbFunction*)(new nbind::cbFunction(cb));
        set_user_data(callback);
        return (*this);
    }
    Signal& update(int val) {
        mapper::Signal::update(val);
        return (*this);
    }
    const char *name()
    {
        return (const char*)(*this);
    }
};

void update_handler(mapper_signal sig, mapper_id instance, const void *value,
                    int count, mapper_timetag_t *tt)
{
        // get user data
    void *user_data = mapper_signal_user_data(sig);
    if (!user_data)
        return;
    nbind::cbFunction *callback = (nbind::cbFunction*)user_data;
    (*callback)(Signal(sig), *(int*)value);
};

class Device : public mapper::Device
{
public:
    Device(const char *name) : mapper::Device(name) {}
    ~Device()
    {
        printf("destroying device!\n");
        mapper::Device::~Device();
    }
    Signal add_input_signal(const char *name, int length, const char *type,
                            nbind::cbFunction& cb)
    {
        void *callback = new nbind::cbFunction(cb);
        return mapper::Device::add_input_signal(name, length, type[0], 0, 0, 0,
                                                update_handler, callback);
    }
    Signal add_output_signal(const char *name, int length, const char *type)
    {
        return mapper::Device::add_output_signal(name, length, type[0], 0, 0, 0);
    }
    bool ready()
        { return mapper::Device::ready(); }
    int poll(int block_ms)
        { return mapper::Device::poll(block_ms); }
};

class Link : public mapper::Link
{

};

class Map : public mapper::Map
{
public:
    Map(Signal source, Signal destination) : mapper::Map(source, destination) {}

    Map& push()
    {
        mapper::Map::push();
        return (*this);
    }
};

namespace mapper {
    NBIND_CLASS(Network) {
        construct<const char *, const char *, int>();
        construct<const char *, const char *>();
        construct<const char *>();
        construct<>();
        method(interface);
    //        method(ip4);
        method(group);
        method(port);
    }

    NBIND_CLASS(Timetag) {
        construct<unsigned long int, unsigned long int>();
        construct<double>();
        construct<>();
        method(sec);
        method(frac);
        method(now);
    //        method(double);
    }
} // namespace mapper

NBIND_CLASS(Device) {
//    inherit(mapper::Device);
//    construct<const char *, int, Network&>();
    construct<const char *>();
////        destruct();
////        method(add_signal);
    method(add_input_signal);
    method(add_output_signal);
//    method(remove_signal);
////        method(property);
////        method(set_property);
////        method(remove_property);
//    method(num_properties);
//    method(clear_staged_properties);
//    method(push);
//    method(network);
//    method(num_signals);
//    method(num_links);
//    method(num_maps);
////        method(signal);
////        method(set_link_callback);
////        method(set_map_callback);
//    method(link);
////        method(links);
////        method(maps);
    method(poll);
    method(ready);
//    method(name);
//    method(id);
//    method(port);
//    method(ordinal);
////        method(start_queue);
////        method(send_queue);
}

NBIND_CLASS(Signal) {
//    inherit(mapper::Signal);
//    method(device);
////        method(maps);
////        method(property);
////        method(set_property);
////        method(remove_property);
////        method(id);
    method(name);
////        method(direction);
////        method(type);
////        method(length);
    method(update);
////        method(value);
    method(set_callback);
////        method(num_maps);
////        method(set_minimum);
////        method(set_maximum);
////        method(set_rate);
}

//NBIND_CLASS(Link) {
//    method(clear_staged_properties);
//    method(push);
//    method(device);
//    method(id);
//    method(num_maps);
//    method(num_properties);
////        method(property);
////        method(set_property);
////        method(remove_property);
////        method(maps);
//}

NBIND_CLASS(Map) {
////        construct<const Map>();
    construct<Signal, Signal>();
//    construct<std::vector<Signal>&, Signal>();
//    method(clear_staged_properties);
    method(push);
//    method(refresh);
//    method(release);
//    method(num_sources);
//    method(ready);
//    method(mode);
//    method(set_mode);
//    method(expression);
//    method(set_expression);
//    method(muted);
//    method(set_muted);
//    method(process_location);
//    method(set_process_location);
//    method(num_properties);
////        method(property);
////        method(set_property);
////        method(remove_property);
//    method(id);
}

//NBIND_CLASS(Database) {
//    construct<int>();
//    construct<>();
//    method(poll);
////        method(flush);
//    method(request_devices);
////        method(subscribe);
////        method(unsubscribe);
////        method(add_device_callback);
////        method(remove_device_callback);
//    method(num_devices);
////        method(device);
////        method(devices);
////        method(add_signal_callback);
////        method(remove_signal_callback);
//    method(num_signals);
//    method(signal);
////        method(signals);
////        method(add_link_callback);
////        method(remove_link_callback);
//    method(num_links);
//    method(link);
////        method(links);
////        method(add_map_callback);
////        method(remove_map_callback);
//    method(num_maps);
//    method(map);
////        method(maps);
//}
