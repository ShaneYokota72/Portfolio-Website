class Person {

    constructor(a, g) {
        this.#age = a;
        this.#gender = g;
        this.code = a+5;
    }

    give(){
        this.#giveinfo();
    }
    #giveinfo() {
        console.log(this.#age + ' ' + this.#gender);
    }
    calc(){
        console.log(this.#age - 5);
    }
    code;
    #age;
    #gender;
}

let people = [];

me = new Person(18, "male");
people.push(me);
console.log(me.code);
you = new Person(18, "female");
people.push(you);

me.give();
for(let i=0; i<people.length; i++){
    people[i].give();
}

function transformArray(array, width) {
    const matrix = new Array(array.length / width).fill().map(() => new Array(width));
    let row = 0;
    let col = 0;
    for (let i = 0; i < array.length; i++) {
        matrix[row][col] = array[i];
        col++;
        if (col === width) {
            col = 0;
            row++;
        }
    }
    return matrix;
}
  
  // example usage
  /* const array = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27];
  const matrix = transformArray(array, 3);
  console.log(matrix);
  console.log(matrix[2][2]); */
  // Output: [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
  
 /*  const deque = [];

  // add an element to the front of the deque
  deque.unshift(1);
  console.log(deque); // [1]
  
  // add an element to the end of the deque
  deque.push(2);
  console.log(deque); // [1, 2]
  
  // remove an element from the front of the deque
  deque.shift();
  console.log(deque); // [2]
  
  // remove an element from the end of the deque
  deque.pop();
  console.log(deque); // []
   */