# Node.js bindings for libmapper

libmapper-node uses the libmapper C++ headers and the
[nbind](https://github.com/charto/nbind) project to build bindings for Node.js.

Install:

    npm install --save nbind autogypi node-gyp

To compile:

    npm run -- node-gyp configure build

