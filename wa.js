let add;

function loadwasm(filename){
    return fetch(filename)
    .then(response => response.arrayBuffer())
    .then(bits => WebAssembly.compile(bits))
    .then(module => { return new WebAssembly.Instance(module)});
};

loadwasm('max_min_finder.wasm')
.then(instance=> {
    findmax = instance.exports._Z12find_max_miniii
})