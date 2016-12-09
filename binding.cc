#include <mapper/mapper_cpp.h>
#include "nbind/api.h"  // for nbind::cbFunction type
#include "nbind/nbind.h"

void device_link_handler(mapper_device dev, mapper_link link,
                         mapper_record_action action);
void device_map_handler(mapper_device dev, mapper_map map,
                        mapper_record_action action);
void db_device_handler(mapper_database db, mapper_device dev,
                       mapper_record_action action, const void *user);
void db_signal_handler(mapper_database db, mapper_signal sig,
                       mapper_record_action action, const void *user);
void db_link_handler(mapper_database db, mapper_link link,
                     mapper_record_action action, const void *user);
void db_map_handler(mapper_database db, mapper_map map,
                    mapper_record_action action, const void *user);

class Signal : public mapper::Signal
{
public:
    Signal(mapper::Signal sig) : mapper::Signal(sig) {}

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

    Signal& clear_staged_properties()
        { mapper::Signal::clear_staged_properties(); return (*this); }
    mapper_direction direction()
        { return mapper::Signal::direction(); }
    mapper_id id()
        { return mapper::Signal::id(); }
    const char *name()
        { return (const char*)(*this); }
    int length()
        { return mapper::Signal::length(); }
    int num_maps()
        { return mapper::Signal::num_maps(); }
    float rate()
        { return mapper::Signal::rate(); }
    char type()
        { return mapper::Signal::type(); }

    void set_rate(float rate)
        { mapper::Signal::set_rate(rate); }
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
    Device(mapper_device dev) : mapper::Device(dev) {}
    Device(const char *name, mapper::Network& net) : mapper::Device(name, 0, net)
        { mapper::Device::set_user_data(this); }
    Device(const char *name) : mapper::Device(name)
        { mapper::Device::set_user_data(this); }
    ~Device()
    {
        printf("destroying device!\n");
        mapper::Device::~Device();
    }
    Signal add_signal(mapper_direction dir, int num_instances, const char *name,
                      int length, const char *type, nbind::cbFunction& cb)
    {
        void *callback = new nbind::cbFunction(cb);
        return mapper::Device::add_signal(dir, num_instances, name, length,
                                          type[0], 0, 0, 0, update_handler,
                                          callback);
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
    Device& remove_signal(Signal& sig)
        { mapper::Device::remove_signal(sig); return (*this); }

    Device& clear_staged_properties()
        { mapper::Device::clear_staged_properties(); return (*this); }
    mapper_id id()
        { return mapper::Device::id(); }
    const char *name()
        { return (const char*)(*this); }
    int num_properties()
        { return mapper::Device::num_properties(); };
    int num_signals(mapper_direction dir)
        { return mapper::Device::num_signals(dir); };
    int num_links(mapper_direction dir)
        { return mapper::Device::num_links(dir); };
    int num_maps(mapper_direction dir)
        { return mapper::Device::num_maps(dir); };
    int ordinal()
        { return mapper::Device::ordinal(); }
    int port()
        { return mapper::Device::port(); }
    Device& push()
        { mapper::Device::push(); return (*this); }
    int poll(int block_ms)
        { return mapper::Device::poll(block_ms); }
    bool ready()
        { return mapper::Device::ready(); }

    Device& set_link_callback(nbind::cbFunction& cb)
    {
        if (link_cb)
            delete link_cb;
        link_cb = new nbind::cbFunction(cb);
        mapper::Device::set_link_callback(device_link_handler);
        return (*this);
    }
    Device& set_map_callback(nbind::cbFunction& cb)
    {
        if (map_cb)
            delete map_cb;
        map_cb = new nbind::cbFunction(cb);
        mapper::Device::set_map_callback(device_map_handler);
        return (*this);
    }
//private:
    nbind::cbFunction *link_cb = 0;
    nbind::cbFunction *map_cb = 0;
};

class Link : public mapper::Link
{
public:
    Link(mapper_link link) : mapper::Link(link) {}
    Link& clear_staged_properties()
        { mapper::Link::clear_staged_properties(); return (*this); }
    mapper_id id()
        { return mapper::Link::id(); }
    int num_maps()
        { return mapper::Link::num_maps(); }
    int num_properties()
        { return mapper::Link::num_properties(); }
    Link& push()
        { mapper::Link::push(); return (*this); }
};

class Map : public mapper::Map
{
public:
    Map(mapper_map map) : mapper::Map(map) {}
    Map(Signal source, Signal destination) : mapper::Map(source, destination) {}

    Map& clear_staged_properties()
        { mapper::Map::clear_staged_properties(); return (*this); }
    const char *expression()
        { return mapper::Map::expression(); }
    mapper_id id()
        { return mapper::Map::id(); }
    mapper_mode mode()
        { return mapper::Map::mode(); }
    bool muted()
        { return mapper::Map::muted(); }
    int num_properties()
        { return mapper::Map::num_properties(); }
    int num_sources()
        { return mapper::Map::num_sources(); }
    Map& push()
        { mapper::Map::push(); return (*this); }
    mapper_location process_location()
        { return mapper::Map::process_location(); }
    bool ready()
        { return mapper::Map::ready(); }
    Map& refresh()
        { mapper::Map::refresh(); return (*this); }
    void release()
        { mapper::Map::release(); }
    Map& set_expression(const char * expr)
        { mapper::Map::set_expression(expr); return (*this); }
    Map& set_mode(mapper_mode mode)
        { mapper::Map::set_mode(mode); return (*this); }
    Map& set_muted(bool muted)
        { mapper::Map::set_muted(muted); return (*this); }
    Map& set_process_location(mapper_location loc)
        { mapper::Map::set_process_location(loc); return (*this); }
};

void device_link_handler(mapper_device dev, mapper_link link,
                         mapper_record_action action)
{
    // get user data
    void *user_data = mapper_device_user_data(dev);
    if (!user_data)
        return;
    Device* cpp_dev = (Device*)user_data;
    (*cpp_dev->link_cb)(Link(link), action);
}

void device_map_handler(mapper_device dev, mapper_map map,
                        mapper_record_action action)
{
    // get user data
    void *user_data = mapper_device_user_data(dev);
    if (!user_data)
        return;
    Device* cpp_dev = (Device*)user_data;
    (*cpp_dev->map_cb)(Map(map), action);
}

class Database : public mapper::Database
{
public:
    Database(int flags) : mapper::Database(flags) {}
    Database() : mapper::Database() {}

    int poll(int block_ms)
        { return mapper::Database::poll(block_ms); }
    Database& flush(int timeout_sec, int quiet)
        { mapper::Database::flush(timeout_sec, quiet); return (*this); }
    Database& request_devices()
        { mapper::Database::request_devices(); return (*this); }
    int num_devices()
        { return mapper::Database::num_devices(); }
    int num_signals(mapper_direction dir)
        { return mapper::Database::num_signals(dir); }
    int num_links()
        { return mapper::Database::num_links(); }
    int num_maps()
        { return mapper::Database::num_maps(); }


    Database& add_device_callback(nbind::cbFunction& cb)
    {
        void *callback = new nbind::cbFunction(cb);
        mapper::Database::add_device_callback(db_device_handler, callback);
        return (*this);
    }
    Database& add_signal_callback(nbind::cbFunction& cb)
    {
        void *callback = new nbind::cbFunction(cb);
        mapper::Database::add_signal_callback(db_signal_handler, callback);
        return (*this);
    }
    Database& add_link_callback(nbind::cbFunction& cb)
    {
        void *callback = new nbind::cbFunction(cb);
        mapper::Database::add_link_callback(db_link_handler, callback);
        return (*this);
    }
    Database& add_map_callback(nbind::cbFunction& cb)
    {
        void *callback = new nbind::cbFunction(cb);
        mapper::Database::add_map_callback(db_map_handler, callback);
        return (*this);
    }
};

void db_device_handler(mapper_database db, mapper_device dev,
                       mapper_record_action action, const void *user)
{
    if (!user)
        return;
    nbind::cbFunction *callback = (nbind::cbFunction*)user;
    (*callback)(Device(dev), action);
}

void db_signal_handler(mapper_database db, mapper_signal sig,
                       mapper_record_action action, const void *user)
{
    if (!user)
        return;
    nbind::cbFunction *callback = (nbind::cbFunction*)user;
    (*callback)(Signal(sig), action);
}

void db_link_handler(mapper_database db, mapper_link link,
                     mapper_record_action action, const void *user)
{
    if (!user)
        return;
    nbind::cbFunction *callback = (nbind::cbFunction*)user;
    (*callback)(Link(link), action);
}

void db_map_handler(mapper_database db, mapper_map map,
                    mapper_record_action action, const void *user)
{
    if (!user)
        return;
    nbind::cbFunction *callback = (nbind::cbFunction*)user;
    (*callback)(Map(map), action);
}

namespace mapper {
    NBIND_CLASS(Network) {
        construct<const char *, const char *, int>();
        construct<const char *, const char *>();
        construct<const char *>();
        construct<>();

        getter(interface);
//        getter(ip4);
        getter(group);
        getter(port);
    }

    NBIND_CLASS(Timetag) {
        construct<unsigned long int, unsigned long int>();
        construct<double>();
        construct<>();

        getset(sec, set_sec);
        getset(frac, set_frac);
        method(now);
    //        method(double);
    }
} // namespace mapper

NBIND_CLASS(Device) {
    construct<const char *, mapper::Network&>();
    construct<const char *>();
    method(add_signal);
    method(add_input_signal);
    method(add_output_signal);
    method(remove_signal);
//    method(property);
//    method(set_property);
//    method(remove_property);
    getter(num_properties);
    method(clear_staged_properties);
    method(push);
//    method(network);
    method(num_signals);
    method(num_links);
    method(num_maps);
//    method(signal);
    method(set_link_callback);
    method(set_map_callback);
//    method(link);
//    method(links);
//    method(maps);
    method(poll);
    getter(ready);
    getter(name);
    getter(id);
    getter(port);
    getter(ordinal);
//    method(start_queue);
//    method(send_queue);
}

NBIND_CLASS(Signal) {
//    method(device);
//    method(maps);
//    method(property);
//    method(set_property);
//    method(remove_property);
    getter(id);
    getter(name);
    getter(direction);
    getter(type);
    getter(length);
    method(update);
//    method(value);
    method(set_callback);
    getter(num_maps);
//    getset(minimum, set_minimum);
//    getset(maximum, set_maximum);
    getset(rate, set_rate);
    method(clear_staged_properties);
}

NBIND_CLASS(Link) {
    method(clear_staged_properties);
    method(push);
//    method(device);
    getter(id);
    getter(num_maps);
    getter(num_properties);
//    method(property);
//    method(set_property);
//    method(remove_property);
//    method(maps);
}

NBIND_CLASS(Map) {
////        construct<const Map>();
    construct<Signal, Signal>();
//    construct<std::vector<Signal>&, Signal>();
    method(clear_staged_properties);
    method(push);
    method(refresh);
    method(release);
    method(num_sources);
    method(ready);
    getset(mode, set_mode);
    getset(expression, set_expression);
    getset(muted, set_muted);
    getset(process_location, set_process_location);
    method(num_properties);
//    method(property);
//    method(set_property);
//    method(remove_property);
    getter(id);
}

NBIND_CLASS(Database) {
    construct<int>();
    construct<>();
    method(poll);
    method(flush);
    method(request_devices);
//    method(subscribe);
//    method(unsubscribe);
    method(add_device_callback);
//    method(remove_device_callback);
    method(num_devices);
//    method(device);
//    method(devices);
//    method(add_signal_callback);
//    method(remove_signal_callback);
    method(num_signals);
//    method(signal);
//    method(signals);
//    method(add_link_callback);
//    method(remove_link_callback);
    method(num_links);
//    method(link);
//    method(links);
//    method(add_map_callback);
//    method(remove_map_callback);
    method(num_maps);
//    method(map);
//    method(maps);
}
