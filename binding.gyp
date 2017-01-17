{
    "targets": [
        {
            'target_name': 'mapper',
            'sources': [ 'binding.cc' ],
			'includes': [ 'auto.gypi' ],
            'include_dirs': [
                             '<!@(pkg-config libmapper --cflags-only-I | sed s/-I//g)',
                             '/opt/local/include',
                             '/usr/local/include',
                            ],
            'libraries': [
                          '<!@(pkg-config libmapper --libs)',
                          '-L/opt/local/lib',
                          '-L/usr/local/lib',
                         ]
		}
	],
	"includes": [
		"auto-top.gypi"
	]
}


