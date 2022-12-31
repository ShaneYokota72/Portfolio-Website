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
let ok = false;
console.log(ok);

me = new Person(18, "male");
me.give();
me.calc();
  