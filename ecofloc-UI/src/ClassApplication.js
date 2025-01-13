class Application {
    constructor(name, listPid, category, color){
        this.name = name;
        this.listePid = listPid;
        this.category = category;
        this.color = color;
    }

    getName(){
        return this.name;
    }
    getListePid(){
        return this.listePid;
    }
    getCategory(){
        return this.category;
    }
    getColor(){
        return this.color;
    }
}