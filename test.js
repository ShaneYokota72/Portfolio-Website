let arr = [0,1,2];
function fix(array){
    for(let i=0; i<array.length; i++){
        if(array[i]<5){
            array[i] = 10;
        }
    }
}
fix(arr);
console.log(arr[0] + arr[1] + arr[2]);