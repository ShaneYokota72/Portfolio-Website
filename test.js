class Person {

    constructor(a, g) {
        this.#age = a;
        this.#gender = g;
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
    #age;
    #gender;
}

/* let people = [];

me = new Person(18, "male");
people.push(me);
you = new Person(18, "female");
people.push(you);

me.give();
for(let i=0; i<people.length; i++){
    people[i].give();
} */

function transformArray(array, width) {
    // create an empty 2D array (matrix) with the same number of rows as the original array
    // and the specified number of columns
    const matrix = new Array(array.length / width).fill().map(() => new Array(width));
  
    // loop through the elements of the original array and assign them to the corresponding elements
    // in the 2D array
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
  
    // return the 2D array
    return matrix;
  }
  
  // example usage
  const array = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27];
  const matrix = transformArray(array, 3);
  console.log(matrix);
  console.log(matrix[2][2]);
  // Output: [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
  