#ifndef GAMES_EH
#define GAMES_EH
#include "game.h"
#include <iomanip>
#include <strstream>
using namespace std;


template<typename TK>
class GamesEH:public GameRecord{
public:
    GamesEH() : GameRecord(){}
    GamesEH(int key, int rank, const char* gameTitle, const char* platform, int year,
            const char* genre, const char* publisher, double northAmerica,
            double europe, double japan, double restOfWorld,
            double global, double review) : GameRecord() {
        this->key = key;
        this->Rank = rank;
        strcpy(this->GameTitle, gameTitle);
        strcpy(this->Platform, platform);
        this->Year = year;
        strcpy(this->Genre, genre);
        strcpy(this->Publisher, publisher);
        this->NorthAmerica = northAmerica;
        this->Europe = europe;
        this->Japan = japan;
        this->RestOfWorld = restOfWorld;
        this->Global = global;
        this->Review = review;
    }

    GamesEH(TK key, string keyType): GamesEH(0, 0, "", "", 0, "", "", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0){
        if (keyType == "index") {
            this->key = key;
        } else if (keyType == "GameTitle") {
            //strncpy(GameTitle, key, sizeof(this->GameTitle) - 1);
            //this->GameTitle[sizeof(this->GameTitle) - 1] = '\0';
        }else{
            cout<<"ERROR"<<endl;
            cout<<keyType<<endl;
        }

        //Extender para todos los campos
    }


    string Key() const{
        ostrstream key;
        key<<this->key<<ends;
        return key.str();
    }

    void showData() const{
        cout << "Key: " << key << endl;
        cout << "Rank: " << Rank << endl;
        cout << "Game Title: " << GameTitle << endl;
        cout << "Platform: " << Platform << endl;
        cout << "Year: " << Year << endl;
        cout << "Genre: " << Genre << endl;
        cout << "Publisher: " << Publisher << endl;
        cout << fixed << setprecision(2);
        cout << "North America Sales: $" << NorthAmerica << "M" << endl;
        cout << "Europe Sales: $" << Europe << "M" << endl;
        cout << "Japan Sales: $" << Japan << "M" << endl;
        cout << "Rest of World Sales: $" << RestOfWorld << "M" << endl;
        cout << "Global Sales: $" << Global << "M" << endl;
        cout << "Review Score: " << Review << "/10" << endl;

    }
    GamesEH& operator=(const GamesEH& other) {
        if (this != &other) {
            key = other.key;
            Rank = other.Rank;
            strncpy(GameTitle, other.GameTitle, sizeof(this->GameTitle) - 1);
            this->GameTitle[sizeof(this->GameTitle) - 1] = '\0';
            strncpy(Platform, other.Platform, sizeof(this->Platform) - 1);
            this->Platform[sizeof(this->Platform) - 1] = '\0';
            Year = other.Year;
            strncpy(Genre, other.Genre, sizeof(this->Genre) - 1);
            this->Genre[sizeof(this->Genre) - 1] = '\0';
            strncpy(Publisher, other.Publisher, sizeof(this->Publisher) - 1);
            this->Publisher[sizeof(this->Publisher) - 1] = '\0';
            NorthAmerica = other.NorthAmerica;
            Europe = other.Europe;
            Japan = other.Japan;
            RestOfWorld = other.RestOfWorld;
            Global = other.Global;
            Review = other.Review;
        }
        return *this;

    }

};
template <typename K>
ostream& operator<<(ostream& os, const GamesEH<K>& record) {
    os << "Key: " << record.key << endl;
    os << "Rank: " << record.Rank << endl;
    os << "Game Title: " << record.GameTitle << endl;
    os << "Platform: " << record.Platform << endl;
    os << "Year: " << record.Year << endl;
    os << "Genre: " << record.Genre << endl;
    os << "Publisher: " << record.Publisher << endl;
    os << fixed << setprecision(2);
    os << "North America Sales: $" << record.NorthAmerica << "M" << endl;
    os << "Europe Sales: $" << record.Europe << "M" << endl;
    os << "Japan Sales: $" << record.Japan << "M" << endl;
    os << "Rest of World Sales: $" << record.RestOfWorld << "M" << endl;
    os << "Global Sales: $" << record.Global << "M" << endl;
    os << "Review Score: " << record.Review << "/10" << endl;
    return os;

}

#endif //EXTENDIBLE_HASHING_GAMESEH_H
