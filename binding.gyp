{
    "targets": [
        {
            'target_name': 'mapper',
            'sources': [ 'binding.cc' ],
			'includes': [ 'auto.gypi' ],
            'include_dirs': [
                             '<!@(pkg-config libmapper-0 --cflags-only-I | sed s/-I//g)',
                             '/opt/local/include',
                             '/usr/local/include',
                            ],
            'libraries': [
                          '<!@(pkg-config libmapper-0 --libs)',
                          '-L/opt/local/lib',
                          '-L/usr/local/lib',
                         ]
		}
	],
	"includes": [
		"auto-top.gypi"
	]
}


