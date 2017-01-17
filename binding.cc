#include <mapper/mapper.h>
#include "nbind/api.h"  // for nbind::cbFunction type
#include "nbind/nbind.h"

#define MAPPER_TYPE(NAME) mapper_ ## NAME
#define QUERY_FUNC(OBJ, FUNC) mapper_ ## OBJ ## _query_ ## FUNC

#define QUERY_METHODS(CLASS_NAME, NAME)                                     \
    Query(MAPPER_TYPE(NAME*) query)                                         \
        { _query = query; }                                                 \
    /* override copy constructor */                                         \
    Query(const Query& orig)                                                \
        { _query = QUERY_FUNC(NAME, copy)(orig._query); }                   \
    ~Query()                                                                \
        { QUERY_FUNC(NAME, done)(_query); }                                 \
    operator MAPPER_TYPE(NAME)*() const                                     \
        { return _query; }                                                  \
    bool operator==(const Query& rhs)                                       \
        { return (_query == rhs._query); }                                  \
    bool operator!=(const Query& rhs)                                       \
        { return (_query != rhs._query); }                                  \
    Query& operator++()                                                     \
    {                                                                       \
        if (_query)                                                         \
            _query = QUERY_FUNC(NAME, next)(_query);                        \
        return (*this);                                                     \
    }                                                                       \
    Query operator++(int)                                                   \
        { Query tmp(*this); operator++(); return tmp; }                     \
    CLASS_NAME operator*()                                                  \
        { return CLASS_NAME(*_query); }                                     \
    Query begin()                                                           \
        { return (*this); }                                                 \
    Query end()                                                             \
        { return Query(0); }                                                \
                                                                            \
    /* Combination functions */                                             \
    Query& join(const Query& rhs)                                           \
    {                                                                       \
        /* need to use copy of rhs query */                                 \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        _query = QUERY_FUNC(NAME, union)(_query, rhs_cpy);                  \
        return (*this);                                                     \
    }                                                                       \
    Query& intersect(const Query& rhs)                                      \
    {                                                                       \
        /* need to use copy of rhs query */                                 \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        _query = QUERY_FUNC(NAME, intersection)(_query, rhs_cpy);           \
        return (*this);                                                     \
    }                                                                       \
    Query& subtract(const Query& rhs)                                       \
    {                                                                       \
        /* need to use copy of rhs query */                                 \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        _query = QUERY_FUNC(NAME, difference)(_query, rhs_cpy);             \
        return (*this);                                                     \
    }                                                                       \
    Query operator+(const Query& rhs) const                                 \
    {                                                                       \
        /* need to use copies of both queries */                            \
        MAPPER_TYPE(NAME) *lhs_cpy = QUERY_FUNC(NAME, copy)(_query);        \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        return Query(QUERY_FUNC(NAME, union)(lhs_cpy, rhs_cpy));            \
    }                                                                       \
    Query operator*(const Query& rhs) const                                 \
    {                                                                       \
        /* need to use copies of both queries */                            \
        MAPPER_TYPE(NAME) *lhs_cpy = QUERY_FUNC(NAME, copy)(_query);        \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        return Query(QUERY_FUNC(NAME, intersection)(lhs_cpy, rhs_cpy));     \
    }                                                                       \
    Query operator-(const Query& rhs) const                                 \
    {                                                                       \
        /* need to use copies of both queries */                            \
        MAPPER_TYPE(NAME) *lhs_cpy = QUERY_FUNC(NAME, copy)(_query);        \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        return Query(QUERY_FUNC(NAME, difference)(lhs_cpy, rhs_cpy));       \
    }                                                                       \
    Query& operator+=(const Query& rhs)                                     \
    {                                                                       \
        /* need to use copy of rhs query */                                 \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        _query = QUERY_FUNC(NAME, union)(_query, rhs_cpy);                  \
        return (*this);                                                     \
    }                                                                       \
    Query& operator*=(const Query& rhs)                                     \
    {                                                                       \
        /* need to use copy of rhs query */                                 \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        _query = QUERY_FUNC(NAME, intersection)(_query, rhs_cpy);           \
        return (*this);                                                     \
    }                                                                       \
    Query& operator-=(const Query& rhs)                                     \
    {                                                                       \
        /* need to use copy of rhs query */                                 \
        MAPPER_TYPE(NAME) *rhs_cpy = QUERY_FUNC(NAME, copy)(rhs._query);    \
        _query = QUERY_FUNC(NAME, difference)(_query, rhs_cpy);             \
        return (*this);                                                     \
    }                                                                       \
                                                                            \
    CLASS_NAME operator [] (int idx)                                        \
        { return CLASS_NAME(QUERY_FUNC(NAME, index)(_query, idx)); }        \
                                                                            \
    operator std::vector<CLASS_NAME>() const                                \
    {                                                                       \
        std::vector<CLASS_NAME> vec;                                        \
        /* use a copy */                                                    \
        MAPPER_TYPE(NAME) *cpy = QUERY_FUNC(NAME, copy)(_query);            \
        while (cpy) {                                                       \
            vec.push_back(CLASS_NAME(*cpy));                                \
            cpy = QUERY_FUNC(NAME, next)(cpy);                              \
        }                                                                   \
        return vec;                                                         \
    }                                                                       \
                                                                            \
    Query& set_user_data(void *user_data)                                   \
    {                                                                       \
        /* use a copy */                                                    \
        MAPPER_TYPE(NAME) *cpy = QUERY_FUNC(NAME, copy)(_query);            \
        while (cpy) {                                                       \
            mapper_ ## NAME ## _set_user_data(*cpy, user_data);             \
            cpy = QUERY_FUNC(NAME, next)(cpy);                              \
        }                                                                   \
        return (*this);                                                     \
    }                                                                       \
    Query& push()                                                           \
    {                                                                       \
        /* use a copy */                                                    \
        MAPPER_TYPE(NAME) *cpy = QUERY_FUNC(NAME, copy)(_query);            \
        while (cpy) {                                                       \
            mapper_ ## NAME ## _push(*cpy);                                 \
            cpy = QUERY_FUNC(NAME, next)(cpy);                              \
        }                                                                   \
        return (*this);                                                     \
    }                                                                       \

// handler prototypes
void update_handler(mapper_signal sig, mapper_id instance, const void *value,
                    int count, mapper_timetag_t *tt);
void device_link_handler(mapper_device dev, mapper_link link,
                         mapper_record_event event);
void device_map_handler(mapper_device dev, mapper_map map,
                        mapper_record_event event);
void db_device_handler(mapper_database db, mapper_device dev,
                       mapper_record_event event, const void *user);
void db_signal_handler(mapper_database db, mapper_signal sig,
                       mapper_record_event event, const void *user);
void db_link_handler(mapper_database db, mapper_link link,
                     mapper_record_event event, const void *user);
void db_map_handler(mapper_database db, mapper_map map,
                    mapper_record_event event, const void *user);


namespace mapper {
    class Device;
    class Signal;
    class Map;
    class Link;
    class Database;

    class Network
    {
    public:
        Network(const char *iface=0, const char *group=0, int port=0)
            { _net = mapper_network_new(iface, group, port); _owned = 1; }
        ~Network()
            { if (_owned && _net) mapper_network_free(_net); }
        operator mapper_network() const
            { return _net; }
        const char *interface() const
            { return mapper_network_interface(_net); }
        const char *group() const
            { return mapper_network_group(_net); }
        int port() const
            { return mapper_network_port(_net); }
    protected:
        friend class Device;
        friend class Database;
        Network(mapper_network net)
            { _net = net; _owned = 0; }
    private:
        mapper_network _net;
        int _owned;
    };

    class Timetag
    {
    public:
        Timetag(unsigned long int sec, unsigned long int frac)
            { _tt.sec = sec; _tt.frac = frac; }
        Timetag(double seconds)
            { mapper_timetag_set_double(&_tt, seconds); }
        Timetag()
            { mapper_timetag_now(&_tt); }
        operator mapper_timetag_t*()
            { return &_tt; }
        uint32_t sec()
            { return _tt.sec; }
        Timetag& set_sec(uint32_t sec)
            { _tt.sec = sec; return (*this); }
        uint32_t frac()
            { return _tt.frac; }
        Timetag& set_frac (uint32_t frac)
            { _tt.frac = frac; return (*this); }
        Timetag& now()
            { mapper_timetag_now(&_tt); return (*this); }
        double get_double() const
            { return mapper_timetag_double(_tt); }

    private:
        mapper_timetag_t _tt;
    };

    class Map
    {
    public:
        Map(const Map& orig)
            { _map = orig._map; }
        Map(mapper_map map)
            { _map = map; }
        Map(const Signal& source, const Signal& destination);
//        Map(int num_sources, signal_type sources[],
//            int num_destinations, signal_type destinations[])
//        {
//            mapper_signal cast_dst = destinations[0];
//            mapper_signal cast_src[num_sources];
//            for (int i = 0; i < num_sources; i++) {
//                cast_src[i] = sources[i];
//            }
//            _map = mapper_map_new(num_sources, cast_src, 1, &cast_dst);
//        }
//        template <size_t N, size_t M>
//        Map(std::array<signal_type, N>& sources,
//            std::array<signal_type, M>& destinations)
//        {
//            if (sources.empty() || destinations.empty() || M != 1) {
//                _map = 0;
//                return;
//            }
//            mapper_signal cast[N];
//            for (int i = 0; i < N; i++) {
//                cast[i] = sources.data()[i];
//            }
//            _map = mapper_map_new(N, cast, 1, destinations.data()[0]);
//        }
//        template <typename T>
//        Map(std::vector<T>& sources, std::vector<T>& destinations)
//        {
//            if (!sources.size() || (destinations.size() != 1)) {
//                _map = 0;
//                return;
//            }
//            int num_sources = sources.size();
//            mapper_signal cast[num_sources];
//            for (int i = 0; i < num_sources; i++) {
//                cast[i] = sources.data()[i];
//            }
//            _map = mapper_map_new(num_sources, cast, 1, destinations.data()[0]);
//        }
        operator mapper_map() const
            { return _map; }
        operator bool() const
            { return _map; }
        operator mapper_id() const
            { return mapper_map_id(_map); }
        const Map& push() const
            { mapper_map_push(_map); return (*this); }
        const Map& refresh() const
            { mapper_map_refresh(_map); return (*this); }
        // this function can be const since it only sends the unmap msg
        void release() const
            { mapper_map_release(_map); }
        int num_sources() const
            { return mapper_map_num_sources(_map); }
        int num_destinations() const
            { return mapper_map_num_destinations(_map); }
        bool ready() const
            { return mapper_map_ready(_map); }
        mapper_mode mode() const
            { return mapper_map_mode(_map); }
        Map& set_mode(mapper_mode mode)
            { mapper_map_set_mode(_map, mode); return (*this); }
        const char* expression() const
            { return mapper_map_expression(_map); }
        Map& set_expression(const char *expression)
            { mapper_map_set_expression(_map, expression); return (*this); }
        bool muted() const
            { return mapper_map_muted(_map); }
        Map& set_muted(bool value)
            { mapper_map_set_muted(_map, (int)value); return (*this); }
        mapper_location process_location() const
            { return mapper_map_process_location(_map); }
        Map& set_process_location(mapper_location loc)
            { mapper_map_set_process_location(_map, loc); return (*this); }
        mapper_id id() const
            { return mapper_map_id(_map); }
        Map& set_user_data(void *user_data)
            { mapper_map_set_user_data(_map, user_data); return (*this); }
        void *user_data() const
            { return mapper_map_user_data(_map); }
//        class Query : public std::iterator<std::input_iterator_tag, int>
//        {
//        public:
//            QUERY_METHODS(Map, map);
//
//                // also enable some Map methods
//            Query& release()
//            {
//                    // use a copy
//                mapper_map *cpy = mapper_map_query_copy(_query);
//                while (cpy) {
//                    mapper_map_release(*cpy);
//                    cpy = mapper_map_query_next(cpy);
//                }
//                return (*this);
//            }
//            Query& set_expression(const char *expression)
//            {
//                    // use a copy
//                mapper_map *cpy = mapper_map_query_copy(_query);
//                while (cpy) {
//                    mapper_map_set_expression(*cpy, expression);
//                    cpy = mapper_map_query_next(cpy);
//                }
//                return (*this);
//            }
//            Query& set_mode(mapper_mode mode)
//            {
//                    // use a copy
//                mapper_map *cpy = mapper_map_query_copy(_query);
//                while (cpy) {
//                    mapper_map_set_mode(*cpy, mode);
//                    cpy = mapper_map_query_next(cpy);
//                }
//                return (*this);
//            }
//        private:
//            mapper_map *_query;
//        };
        class Slot
        {
        public:
            Slot(mapper_slot slot)
                { _slot = slot; }
            Slot(const Slot &orig)
                { _slot = orig._slot; }
            operator mapper_slot() const
                { return _slot; }
            inline Signal signal() const;
            mapper_boundary_action bound_min() const
                { return mapper_slot_bound_min(_slot); }
            Slot& set_bound_min(mapper_boundary_action bound_min)
                { mapper_slot_set_bound_min(_slot, bound_min); return (*this); }
            mapper_boundary_action bound_max() const
                { return mapper_slot_bound_max(_slot); }
            Slot& set_bound_max(mapper_boundary_action bound_max)
                { mapper_slot_set_bound_max(_slot, bound_max); return (*this); }
//            Property minimum() const
//            {
//                char type;
//                int length;
//                void *value;
//                mapper_slot_minimum(_slot, &length, &type, &value);
//                if (value)
//                    return Property("minimum", length, type, value);
//                else
//                    return Property("minimum", 0, 0, 0);
//            }
//            Slot& set_minimum(const Property &value)
//            {
//                mapper_slot_set_minimum(_slot, value.length, value.type,
//                                        (void*)(const void*)value);
//                return (*this);
//            }
//            Property maximum() const
//            {
//                char type;
//                int length;
//                void *value;
//                mapper_slot_maximum(_slot, &length, &type, &value);
//                if (value)
//                    return Property("maximum", length, type, value);
//                else
//                    return Property("maximum", 0, 0, 0);
//            }
//            Slot& set_maximum(const Property &value)
//            {
//                mapper_slot_set_maximum(_slot, value.length, value.type,
//                                        (void*)(const void*)value);
//                return (*this);
//            }
            bool calibrating() const
                { return mapper_slot_calibrating(_slot); }
            Slot& set_calibrating(bool value)
            {
                mapper_slot_set_calibrating(_slot, (int)value);
                return (*this);
            }
            bool causes_update() const
                { return mapper_slot_causes_update(_slot); }
            Slot& set_causes_update(bool value)
            {
                mapper_slot_set_causes_update(_slot, (int)value);
                return (*this);
            }
            bool use_instances() const
                { return mapper_slot_use_instances(_slot); }
            Slot& set_use_instances(bool value)
            {
                mapper_slot_set_use_instances(_slot, (int)value);
                return (*this);
            }
        protected:
            friend class Map;
        private:
            mapper_slot _slot;
        };
        Slot destination(int index=0) const
            { return Slot(mapper_map_slot(_map, MAPPER_LOC_DESTINATION, 0)); }
        Slot source(int index=0) const
            { return Slot(mapper_map_slot(_map, MAPPER_LOC_SOURCE, index)); }
    protected:
        friend class Database;
    private:
        mapper_map _map;
    };

    class Link
    {
    public:
        Link(const Link& orig)
            { _link = orig._link; }
        Link(mapper_link link)
            { _link = link; }
        operator mapper_link() const
            { return _link; }
        operator bool() const
            { return _link; }
        operator mapper_id() const
            { return mapper_link_id(_link); }
        const Link& push() const
            { mapper_link_push(_link); return (*this); }
        inline Device device(int idx) const;
        mapper_id id() const
            { return mapper_link_id(_link); }
        int num_maps() const
            { return mapper_link_num_maps(_link); }
//        Map::Query maps() const
//            { return Map::Query(mapper_link_maps(_link)); }
        Link& set_user_data(void *user_data)
            { mapper_link_set_user_data(_link, user_data); return (*this); }
        void *user_data() const
            { return mapper_link_user_data(_link); }
//        class Query : public std::iterator<std::input_iterator_tag, int>
//        {
//        public:
//            QUERY_METHODS(Link, link);
//        private:
//            mapper_link *_query;
//        };
    protected:
        friend class Database;
    private:
        mapper_link _link;
    };

    class Signal
    {
    private:
        mapper_signal _sig;

    public:
        Signal(mapper_signal sig)
            { _sig = sig; }
        Signal(const Signal& orig)
            { _sig = orig._sig; }
        operator mapper_signal() const
            { return _sig; }
        operator bool() const
            { return _sig ? true : false; }
        inline Device device() const;
//        Map::Query maps(mapper_direction dir=MAPPER_DIR_ANY) const
//            { return Map::Query(mapper_signal_maps(_sig, dir)); }

        mapper_id id() const
            { return mapper_signal_id(_sig); }
        const char *name() const
            { return mapper_signal_name(_sig); }
        mapper_direction direction() const
            { return mapper_signal_direction(_sig); }
        char type() const
            { return mapper_signal_type(_sig); }
        int length() const
            { return mapper_signal_length(_sig); }

//        /* Value update functions*/
//        Signal& update(void *value, int count, Timetag tt)
//        {
//            mapper_signal_update(_sig, value, count, *tt);
//            return (*this);
//        }
        Signal& update(int *value, int count, Timetag tt)
        {
            if (mapper_signal_type(_sig) == 'i')
                mapper_signal_update(_sig, value, count, *tt);
            return (*this);
        }
//        Signal& update(float *value, int count, Timetag tt)
//        {
//            if (mapper_signal_type(_sig) == 'f')
//                mapper_signal_update(_sig, value, count, *tt);
//            return (*this);
//        }
//        Signal& update(double *value, int count, Timetag tt)
//        {
//            if (mapper_signal_type(_sig) == 'd')
//                mapper_signal_update(_sig, value, count, *tt);
//            return (*this);
//        }
        Signal& update(int value)
            { return update(&value, 1, 0); }
//        template <typename T>
//        Signal& update(T value)
//            { return update(&value, 1, 0); }
//        template <typename T>
//        Signal& update(T* value)
//        { return update(value, 1, 0); }
//        template <typename T, int count>
//        Signal& update(T* value)
//        { return update(value, count, 0); }
//        template <typename T>
//        Signal& update(T* value, Timetag tt)
//        { return update(value, 1, tt); }
//        template <typename T, size_t N>
//        Signal& update(std::array<T,N> value)
//        { return update(&value[0], N / mapper_signal_length(_sig), 0); }
//        template <typename T>
//        Signal& update(std::vector<T> value, Timetag tt=0)
//        {
//            return update(&value[0],
//                          (int)value.size() / mapper_signal_length(_sig), *tt);
//        }
//        const void *value() const
//        { return mapper_signal_value(_sig, 0); }
//        const void *value(Timetag tt) const
//        { return mapper_signal_value(_sig, (mapper_timetag_t*)tt); }
        int query_remotes() const
            { return mapper_signal_query_remotes(_sig, MAPPER_NOW); }
        int query_remotes(Timetag tt) const
            { return mapper_signal_query_remotes(_sig, *tt); }
        Signal& set_user_data(void *user_data)
            { mapper_signal_set_user_data(_sig, user_data); return (*this); }
        void *user_data() const
            { return mapper_signal_user_data(_sig); }
        Signal& set_callback(nbind::cbFunction& cb)
        {
            void *callback = user_data();
            if (callback)
                delete((nbind::cbFunction*)callback);
            callback = new nbind::cbFunction(cb);
            this->set_user_data(callback);
            mapper_signal_set_callback(_sig, update_handler);
            return (*this);
        }
        int num_maps(mapper_direction dir=MAPPER_DIR_ANY) const
            { return mapper_signal_num_maps(_sig, dir); }
        float rate()
            { return mapper_signal_rate(_sig); }
        Signal& set_rate(int rate)
            { mapper_signal_set_rate(_sig, rate); return (*this); }

        class Instance {
        public:
            Instance(mapper_signal sig, mapper_id id)
                { _sig = sig; _id = id; }
            bool operator == (Instance i)
                { return (_id == i._id); }
            operator mapper_id() const
                { return _id; }
            Instance& update(void *value, int count, Timetag tt)
            {
                mapper_signal_instance_update(_sig, _id, value, count, *tt);
                return (*this);
            }
            Instance& update(int *value, int count, Timetag tt)
            {
                if (mapper_signal_type(_sig) == 'i')
                    mapper_signal_instance_update(_sig, _id, value, count, *tt);
                return (*this);
            }
            Instance& update(float *value, int count, Timetag tt)
            {
                if (mapper_signal_type(_sig) == 'f')
                    mapper_signal_instance_update(_sig, _id, value, count, *tt);
                return (*this);
            }
            Instance& update(double *value, int count, Timetag tt)
            {
                if (mapper_signal_type(_sig) == 'd')
                    mapper_signal_instance_update(_sig, _id, value, count, *tt);
                return (*this);
            }

            void release()
                { mapper_signal_instance_release(_sig, _id, MAPPER_NOW); }
            void release(Timetag tt)
                { mapper_signal_instance_release(_sig, _id, *tt); }

            template <typename T>
            Instance& update(T value)
                { return update(&value, 1, 0); }
            template <typename T>
            Instance& update(T* value, int count=0)
                { return update(value, count, 0); }
            template <typename T>
            Instance& update(T* value, Timetag tt)
                { return update(value, 1, tt); }
            template <typename T, size_t N>
            Instance& update(std::array<T,N> value, Timetag tt=0)
            {
                return update(&value[0], N / mapper_signal_length(_sig), tt);
            }
            template <typename T>
            Instance& update(std::vector<T> value, Timetag tt=0)
            {
                return update(&value[0],
                              value.size() / mapper_signal_length(_sig), tt);
            }

            mapper_id id() const
                { return _id; }

            Instance& set_user_data(void *user_data)
            {
                mapper_signal_instance_set_user_data(_sig, _id, user_data);
                return (*this);
            }
            void *user_data() const
                { return mapper_signal_instance_user_data(_sig, _id); }

            const void *value() const
                { return mapper_signal_instance_value(_sig, _id, 0); }
            const void *value(Timetag tt) const
            {
                mapper_timetag_t *_tt = tt;
                return mapper_signal_instance_value(_sig, _id, _tt);
            }
        protected:
            friend class Signal;
        private:
            mapper_id _id;
            mapper_signal _sig;
        };
        Instance instance()
        {
            mapper_id id = mapper_device_generate_unique_id(mapper_signal_device(_sig));
                // TODO: wait before activating instance?
            mapper_signal_instance_set_user_data(_sig, id, 0);
            return Instance(_sig, id);
        }
        Instance instance(mapper_id id)
        {
                // TODO: wait before activating instance?
            mapper_signal_instance_set_user_data(_sig, id, 0);
            return Instance(_sig, id);
        }
        Signal& reserve_instances(int num, mapper_id *ids = 0)
        {
            mapper_signal_reserve_instances(_sig, num, ids, 0);
            return (*this);
        }
        Signal& reserve_instances(int num, mapper_id *ids, void **user_data)
        {
            mapper_signal_reserve_instances(_sig, num, ids, user_data);
            return (*this);
        }
        Instance active_instance_at_index(int index) const
        {
            return Instance(_sig, mapper_signal_active_instance_id(_sig, index));
        }
        Signal& remove_instance(Instance instance)
            { mapper_signal_remove_instance(_sig, instance._id); return (*this); }
        Instance oldest_active_instance()
        {
            return Instance(_sig,
                            mapper_signal_oldest_active_instance(_sig));
        }
        Instance newest_active_instance()
        {
            return Instance(_sig,
                            mapper_signal_newest_active_instance(_sig));
        }
        int num_instances() const
            { return mapper_signal_num_instances(_sig); }
        int num_active_instances() const
            { return mapper_signal_num_active_instances(_sig); }
        int num_reserved_instances() const
            { return mapper_signal_num_reserved_instances(_sig); }
        Signal& set_instance_stealing_mode(mapper_instance_stealing_type mode)
        {
            mapper_signal_set_instance_stealing_mode(_sig, mode);
            return (*this);
        }
        mapper_instance_stealing_type instance_stealing_mode() const
            { return mapper_signal_instance_stealing_mode(_sig); }
        Signal& set_instance_event_callback(mapper_instance_event_handler h,
                                            int flags)
        {
            mapper_signal_set_instance_event_callback(_sig, h, flags);
            return (*this);
        }

        class Query : public std::iterator<std::input_iterator_tag, int>
        {
        public:
            QUERY_METHODS(Signal, signal);
        private:
            mapper_signal *_query;
        };
    };
}

void update_handler(mapper_signal sig, mapper_id instance, const void *value,
                    int count, mapper_timetag_t *tt)
{
    // get user data
    void *user_data = mapper_signal_user_data(sig);
    if (!user_data)
        return;
    nbind::cbFunction *cb = (nbind::cbFunction*)user_data;
    (*cb)(mapper::Signal(sig), *(int*)value);
};

//class Device : public mapper::Device
//{
//public:
//    class Query : public mapper::Device::Query
//    {
//    public:
//        Query(mapper_device *query) : mapper::Device::Query(query)
//            { printf("Device::Query()\n"); }
//        bool check()
//            { return (*this); }
//        bool next()
//        {
//            printf("Device::Query::next()\n");
//            Device temp = (Device)(mapper::Device)(*this);
//            (*this)++;
//            return temp;
//        }
//        Device deref()
//        {
//            printf("Device::Query::deref()\n");
//            return (Device)(mapper::Device)(*this);
//        }
//    };
//};

namespace mapper {
    class Device
    {
    public:
        Device(const char *name_prefix, int port, const Network& net)
        {
            _dev = mapper_device_new(name_prefix, port, net);
            _db = mapper_device_database(_dev);
            _owned = 1;
            _refcount_ptr = (int*)malloc(sizeof(int));
            *_refcount_ptr = 1;
        }
        Device(const char *name_prefix)
        {
            _dev = mapper_device_new(name_prefix, 0, 0);
            _db = mapper_device_database(_dev);
            _owned = 1;
            _refcount_ptr = (int*)malloc(sizeof(int));
            *_refcount_ptr = 1;
        }
        Device(const Device& orig) {
            if (orig) {
                _dev = orig._dev;
                _db = orig._db;
                _owned = orig._owned;
                _refcount_ptr = orig._refcount_ptr;
                if (_owned)
                    incr_refcount();
            }
        }
        Device(mapper_device dev)
        {
            _dev = dev;
            _db = mapper_device_database(_dev);
            _owned = 0;
            _refcount_ptr = (int*)malloc(sizeof(int));
            *_refcount_ptr = 1;
        }
        ~Device()
        {
            if (_owned && _dev && decr_refcount() <= 0) {
                mapper_device_free(_dev);
                _dev = 0;
                _db = 0;
            }
        }
        operator mapper_device() const
            { return _dev; }
        operator const char*() const
            { return mapper_device_name(_dev); }
        operator mapper_id() const
            { return mapper_device_id(_dev); }

        Device& set_user_data(void *user_data)
            { mapper_device_set_user_data(_dev, user_data); return (*this); }
        void *user_data() const
            { return mapper_device_user_data(_dev); }

        Signal add_signal(mapper_direction dir, int num_instances,
                          const char *name, int length, const char *type,
                          nbind::cbFunction& cb)
        {
            void *callback = new nbind::cbFunction(cb);
            return mapper_device_add_signal(_dev, dir, num_instances,
                                            name, length, type[0], 0, 0, 0,
                                            update_handler, callback);
        }
        Signal add_input_signal(const char *name, int length, const char *type,
                                nbind::cbFunction& cb)
        {
            void *callback = new nbind::cbFunction(cb);
            return mapper_device_add_input_signal(_dev, name, length, type[0],
                                                  0, 0, 0, update_handler,
                                                  callback);
        }
        Signal add_output_signal(const char *name, int length, const char *type)
        {
            return mapper_device_add_output_signal(_dev, name, length, type[0],
                                                   0, 0, 0);
        }
        Device& remove_signal(Signal& sig)
            { mapper_device_remove_signal(_dev, sig); return (*this); }

        const Device& push() const
            { mapper_device_push(_dev); return (*this); }

        Network network() const
            { return Network(mapper_device_network(_dev)); }

        int num_signals(mapper_direction dir=MAPPER_DIR_ANY) const
            { return mapper_device_num_signals(_dev, dir); }
        int num_links(mapper_direction dir=MAPPER_DIR_ANY) const
            { return mapper_device_num_links(_dev, dir); }
        int num_maps(mapper_direction dir=MAPPER_DIR_ANY) const
            { return mapper_device_num_maps(_dev, dir); }

        Signal signal(const char *name)
            { return Signal(mapper_device_signal_by_name(_dev, name)); }
        Signal signal(mapper_id id)
            { return Signal(mapper_device_signal_by_id(_dev, id)); }
        Signal::Query signals(mapper_direction dir=MAPPER_DIR_ANY) const
            { return Signal::Query(mapper_device_signals(_dev, dir)); }

        Device& set_link_callback(nbind::cbFunction& cb)
        {
            if (link_cb)
                delete link_cb;
            link_cb = new nbind::cbFunction(cb);
            mapper_device_set_link_callback(_dev, device_link_handler);
            return (*this);
        }
        Device& set_map_callback(nbind::cbFunction& cb)
        {
            if (map_cb)
                delete map_cb;
            map_cb = new nbind::cbFunction(cb);
            mapper_device_set_map_callback(_dev, device_map_handler);
            return (*this);
        }
        Link link(Device remote)
        {
            return Link(mapper_device_link_by_remote_device(_dev, remote._dev));
        }
//        Link::Query links(mapper_direction dir=MAPPER_DIR_ANY) const
//            { return Link::Query(mapper_device_links(_dev, dir)); }
//        Map::Query maps(mapper_direction dir=MAPPER_DIR_ANY) const
//            { return Map::Query(mapper_device_maps(_dev, dir)); }

        int poll(int block_ms=0) const
            { return mapper_device_poll(_dev, block_ms); }
        int num_fds() const
            { return mapper_device_num_fds(_dev); }
        int fds(int *fds, int num) const
            { return mapper_device_fds(_dev, fds, num); }
        Device& service_fd(int fd)
            { mapper_device_service_fd(_dev, fd); return (*this); }
        bool ready() const
            { return mapper_device_ready(_dev); }
        const char *name() const
            { return mapper_device_name(_dev); }
        mapper_id id() const
            { return mapper_device_id(_dev); }
        int port() const
            { return mapper_device_port(_dev); }
        int ordinal() const
            { return mapper_device_ordinal(_dev); }
        Device& start_queue(Timetag tt)
            { mapper_device_start_queue(_dev, *tt); return (*this); }
        Device& send_queue(Timetag tt)
            { mapper_device_send_queue(_dev, *tt); return (*this); }

//        class Query : public std::iterator<std::input_iterator_tag, int>
//        {
//        public:
//            QUERY_METHODS(Device, device);
//        private:
//            mapper_device *_query;
//        };
    private:
        mapper_device _dev;
        mapper_database _db;
        int _owned;
        int* _refcount_ptr;
        int incr_refcount()
            { return _refcount_ptr ? ++(*_refcount_ptr) : 0; }
        int decr_refcount()
            { return _refcount_ptr ? --(*_refcount_ptr) : 0; }

        nbind::cbFunction *link_cb;
        nbind::cbFunction *map_cb;
    };
}

void device_link_handler(mapper_device dev, mapper_link link,
                         mapper_record_event event)
{
    // get user data
    void *user_data = mapper_device_user_data(dev);
    if (!user_data)
        return;
    nbind::cbFunction *cb = (nbind::cbFunction*)user_data;
    (*cb)(mapper::Link(link), event);
}

void device_map_handler(mapper_device dev, mapper_map map,
                        mapper_record_event event)
{
    // get user data
    void *user_data = mapper_device_user_data(dev);
    if (!user_data)
        return;
    nbind::cbFunction *cb = (nbind::cbFunction*)user_data;
    (*cb)(mapper::Map(map), event);
}

namespace mapper {
    class Database
    {
    public:
        Database(Network& net, int flags = MAPPER_OBJ_ALL)
        {
            _db = mapper_database_new(net, flags);
            _owned = true;
            _refcount_ptr = (int*)malloc(sizeof(int));
            *_refcount_ptr = 1;
        }
        Database(int flags = MAPPER_OBJ_ALL)
        {
            _db = mapper_database_new(0, flags);
            _owned = true;
            _refcount_ptr = (int*)malloc(sizeof(int));
            *_refcount_ptr = 1;
        }
        Database(const Database& orig)
        {
            _db = orig._db;
            _owned = orig._owned;
            _refcount_ptr = orig._refcount_ptr;
            if (_owned)
                incr_refcount();
        }
        Database(mapper_database db)
        {
            _db = db;
            _owned = false;
            _refcount_ptr = (int*)malloc(sizeof(int));
            *_refcount_ptr = 1;
        }
        ~Database()
        {
            if (_owned && _db && decr_refcount() <= 0)
                mapper_database_free(_db);
        }
        int poll(int block_ms=0) const
            { return mapper_database_poll(_db, block_ms); }
        const Database& flush(int timeout_sec, int quiet=0) const
        {
            mapper_database_flush(_db, timeout_sec ?: mapper_database_timeout(_db),
                                  quiet);
            return (*this);
        }

        // subscriptions
        const Database& request_devices() const
            { mapper_database_request_devices(_db); return (*this); }
        const Database& subscribe(const Device& dev, int flags, int timeout)
        {
            mapper_database_subscribe(_db, dev, flags, timeout);
            return (*this);
        }
//        const Database& subscribe(int flags)
//            { mapper_database_subscribe(_db, 0, flags, -1); return (*this); }
        const Database& unsubscribe(const Device& dev)
        {
            mapper_database_unsubscribe(_db, dev);
            return (*this);
        }
        const Database& unsubscribe()
            { mapper_database_unsubscribe(_db, 0); return (*this); }

        // database_devices
        const Database& add_device_callback(nbind::cbFunction& callback) const
        {
            void *cb = new nbind::cbFunction(callback);
            mapper_database_add_device_callback(_db, db_device_handler, cb);
            return (*this);
        }
        const Database& remove_device_callback(nbind::cbFunction& cb) const
        {
            mapper_database_remove_device_callback(_db, db_device_handler, &cb);
            return (*this);
        }

        int num_devices() const
            { return mapper_database_num_devices(_db); }
        Device device(mapper_id id) const
            { return Device(mapper_database_device_by_id(_db, id)); }
        Device device(const char *name) const
            { return Device(mapper_database_device_by_name(_db, name)); }
//        Device::Query devices() const
//            { return Device::Query(mapper_database_devices(_db)); }
//        Device::Query devices(const char *name) const
//        {
//            return Device::Query(mapper_database_devices_by_name(_db, name));
//        }
//        Device::Query devices(const Property& p, mapper_op op) const
//        {
//            return Device::Query(
//                                 mapper_database_devices_by_property(_db, p.name, p.length,
//                                                                     p.type, p.value, op));
//        }
//        inline Device::Query devices(const Property& p) const
//            { return devices(p, MAPPER_OP_EXISTS); }

        // database_signals
        const Database& add_signal_callback(nbind::cbFunction& callback) const
        {
            void *cb = new nbind::cbFunction(callback);
            mapper_database_add_signal_callback(_db, db_signal_handler, cb);
            return (*this);
        }
        const Database& remove_signal_callback(nbind::cbFunction& cb) const
        {
            mapper_database_remove_signal_callback(_db, db_signal_handler, &cb);
            return (*this);
        }

        int num_signals(mapper_direction dir=MAPPER_DIR_ANY) const
            { return mapper_database_num_signals(_db, dir); }
        Signal signal(mapper_id id) const
            { return Signal(mapper_database_signal_by_id(_db, id)); }
//        Signal::Query signals(mapper_direction dir=MAPPER_DIR_ANY) const
//            { return Signal::Query(mapper_database_signals(_db, dir)); }
//        Signal::Query signals(const char *name) const
//        {
//            return Signal::Query(mapper_database_signals_by_name(_db, name));
//        }
//        Signal::Query signals(const Property& p, mapper_op op) const
//        {
//            return Signal::Query(
//                mapper_database_signals_by_property(_db, p.name, p.length,
//                                                    p.type, p.value, op));
//        }
//        inline Signal::Query signals(const Property& p) const
//            { return signals(p, MAPPER_OP_EXISTS); }

        // database links
        const Database& add_link_callback(nbind::cbFunction& callback) const
        {
            void *cb = new nbind::cbFunction(callback);
            mapper_database_add_link_callback(_db, db_link_handler, cb);
            return (*this);
        }
        const Database& remove_link_callback(nbind::cbFunction& cb) const
        {
            mapper_database_remove_link_callback(_db, db_link_handler, &cb);
            return (*this);
        }

        int num_links() const
            { return mapper_database_num_links(_db); }
        Link link(mapper_id id) const
            { return Link(mapper_database_link_by_id(_db, id)); }
//        Link::Query links() const
//            { return Link::Query(mapper_database_links(_db)); }
//        Link::Query links(const Property& p, mapper_op op) const
//        {
//            return Link::Query(
//                               mapper_database_links_by_property(_db, p.name, p.length, p.type,
//                                                                 p.value, op));
//        }
//        inline Link::Query links(const Property& p) const
//            { return links(p, MAPPER_OP_EXISTS); }

        // database maps
        const Database& add_map_callback(nbind::cbFunction& callback) const
        {
            void *cb = new nbind::cbFunction(callback);
            mapper_database_add_map_callback(_db, db_map_handler, cb);
            return (*this);
        }
        const Database& remove_map_callback(nbind::cbFunction& cb) const
        {
            mapper_database_remove_map_callback(_db, db_map_handler, &cb);
            return (*this);
        }

        int num_maps() const
            { return mapper_database_num_maps(_db); }
        Map map(mapper_id id) const
            { return Map(mapper_database_map_by_id(_db, id)); }
//        Map::Query maps() const
//            { return Map::Query(mapper_database_maps(_db)); }
//        Map::Query maps(const Property& p, mapper_op op) const
//        {
//            return Map::Query(
//                mapper_database_maps_by_property(_db, p.name, p.length, p.type,
//                                                 p.value, op));
//        }
//        inline Map::Query maps(const Property& p) const
//            { return maps(p, MAPPER_OP_EXISTS); }
    private:
        mapper_database _db;
        bool _owned;
        int* _refcount_ptr;
        int incr_refcount()
            { return _refcount_ptr ? ++(*_refcount_ptr) : 0; }
        int decr_refcount()
            { return _refcount_ptr ? --(*_refcount_ptr) : 0; }
    };
}

void db_device_handler(mapper_database db, mapper_device dev,
                       mapper_record_event event, const void *user)
{
    if (!user)
        return;
    nbind::cbFunction *cb = (nbind::cbFunction*)user;
    (*cb)(mapper::Device(dev), (int)event);
}

void db_signal_handler(mapper_database db, mapper_signal sig,
                       mapper_record_event event, const void *user)
{
    if (!user)
        return;
    nbind::cbFunction *cb = (nbind::cbFunction*)user;
    (*cb)(mapper::Signal(sig), (int)event);
}

void db_link_handler(mapper_database db, mapper_link link,
                     mapper_record_event event, const void *user)
{
    if (!user)
        return;
    nbind::cbFunction *cb = (nbind::cbFunction*)user;
    (*cb)(mapper::Link(link), (int)event);
}

void db_map_handler(mapper_database db, mapper_map map,
                    mapper_record_event event, const void *user)
{
    if (!user)
        return;
    nbind::cbFunction *cb = (nbind::cbFunction*)user;
    (*cb)(mapper::Map(map), (int)event);
}


namespace mapper {
    Device Link::device(int idx) const
        { return Device(mapper_link_device(_link, idx)); }

    Signal Map::Slot::signal() const
        { return Signal(mapper_slot_signal(_slot)); }

    Device Signal::device() const
        { return Device(mapper_signal_device(_sig)); }

    Map::Map(const Signal& source, const Signal& destination)
    {
        mapper_signal cast_src = source, cast_dst = destination;
        _map = mapper_map_new(1, &cast_src, 1, &cast_dst);
    }
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
        method(get_double);
    }
}

namespace mapper {
    NBIND_CLASS(Device) {
//        construct<const char *, Network&>();
        construct<const char *>();

        method(add_signal);
        method(add_input_signal);
        method(add_output_signal);
        method(remove_signal);
//        getter(num_properties);
//        method(clear_staged_properties);
        method(push);
    //    method(network);
        method(num_signals);
        method(num_links);
        method(num_maps);
    //    method(signal_by_name, "signal");
    //    method(signal_by_id, "signal");
    ////    method(signals);
    //    method(set_link_callback);
    //    method(set_map_callback);
    //    method(link);
    ////    method(links);
    ////    method(maps);
        method(poll);
        getter(ready);
        method(name);
        getter(name);
        getter(id);
        getter(port);
        getter(ordinal);
        method(start_queue);
        method(send_queue);
    }
}
//
//NBIND_CLASS(Device::Query, Query) {
//    construct<mapper_device*>();
//    method(next);
//    method(deref);
//    method(check);
//}

namespace mapper {
    NBIND_CLASS(Signal) {
        method(device);
    //    method(maps);
        getter(id);
        method(name);
        getter(name);
        getter(direction);
        getter(type);
        getter(length);
        multimethod(update, args(int));
    //    method(value);
        method(set_callback);
        method(num_maps);
    //    getset(minimum, set_minimum);
    //    getset(maximum, set_maximum);
        getset(rate, set_rate);
//        method(clear_staged_properties);
    }
}

NBIND_CLASS(mapper::Link, Link) {
//    method(clear_staged_properties);
    method(push);
    method(device);
    getter(id);
    getter(num_maps);
//    getter(num_properties);
//    method(maps);
}

namespace mapper {
    NBIND_CLASS(Map) {
    ////        construct<const Map>();
        construct<Signal, Signal>();
    //    construct<std::vector<Signal>&, Signal>();
    //    method(clear_staged_properties);
        method(push);
        method(refresh);
        method(release);
        method(num_sources);
        method(num_destinations);
        method(ready);
        method(set_mode);
        getset(mode, set_mode);
        method(set_expression);
        getset(expression, set_expression);
        method(set_muted);
        getset(muted, set_muted);
        method(set_process_location);
        getset(process_location, set_process_location);
    //    method(num_properties);
        getter(id);
        method(source);
        method(destination);
    }
}

NBIND_CLASS(mapper::Map::Slot, Slot) {
    getset(bound_min, set_bound_min);
    getset(bound_max, set_bound_max);
//        getset(minimum, set_minimum);
//        getset(maximum, set_maximum);
    getset(calibrating, set_calibrating);
    getset(causes_update, set_causes_update);
    getset(use_instances, set_use_instances);
    method(signal);
}

NBIND_CLASS(mapper::Database, Database) {
    construct<int>();
    construct<>();
    method(poll);
    method(flush);
    method(request_devices);
//    method(subscribe);
//    method(unsubscribe);
    method(add_device_callback);
    method(remove_device_callback);
    method(num_devices);
//    method(device);
//    method(devices);
    method(add_signal_callback);
    method(remove_signal_callback);
    method(num_signals);
//    method(signal);
//    method(signals);
    method(add_link_callback);
    method(remove_link_callback);
    method(num_links);
//    method(link);
//    method(links);
    method(add_map_callback);
    method(remove_map_callback);
    method(num_maps);
//    method(map);
//    method(maps);
}
