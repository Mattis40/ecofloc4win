class Application {
    constructor(name, listePid, categorie, couleur){
        this.name = name;
        this.listePid = listePid;
        this.categorie = categorie;
        this.couleur = couleur;
    }

    getName(){
        return this.name;
    }
    getListePid(){
        return this.listePid;
    }
    getCategorie(){
        return this.categorie;
    }
    getCouleur(){
        return this.couleur;
    }
}