#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

ifstream fin("input.txt");

class Tranzitie
{
    int Nod1;
    int Nod2;
    string litera;
public:
    Tranzitie(int, int, string);
    string get_litera();
    int get_Nod1();
    int get_Nod2();
    friend istream& operator>>(istream&, Tranzitie&);
    friend ostream& operator<<(ostream&, const Tranzitie&);
};

Tranzitie::Tranzitie(int primul_nod, int al_doilea_nod, string caracter)
{
    this->Nod1 = primul_nod;
    this->Nod2 = al_doilea_nod;
    this->litera = caracter;
}

string Tranzitie::get_litera()
{
    return this->litera;
}

int Tranzitie::get_Nod1()
{
    return this->Nod1;
}
int Tranzitie::get_Nod2()
{
    return this->Nod2;
}

istream& operator>>(istream& in, Tranzitie& ob)
{
    in >> ob.Nod1 >> ob.Nod2 >> ob.litera;
    return in;
}

ostream& operator<<(ostream& os, const Tranzitie& ob)
{
    os << ob.Nod1 << ' ' << ob.Nod2 << ' ' << ob.litera;
    return os;
}

class DFA
{
    int number_of_states;
    int initial_state;
    vector<int>states;
    vector<int>final_states;
    
    vector<vector<Tranzitie>>transitions;
 
public:
    string alfabet;
    vector<vector<int>>partitii;
    DFA(int n, int m)
    {
        number_of_states = n;
        transitions.resize(m);
        initial_state = 0;
    }
    void add_transition(int, int, string);
    void add_transition(Tranzitie);
    void add_state(int);
    void add_final_state(int);
    void set_initial_state(int);

    int get_initial_state();

    void clear_states();
    void clear_transitions();
    int get_max_state();
    int get_specified_state(int);
    void print_states();
    void print_transitions();
    void print_final_states();
    vector<int> get_states();

    bool is_final_state(int);
    bool mai_exista_partitii();
    bool check_if_in_same_partition(int, int);
    void construieste_partitii_noi();
    bool se_potriveste_la_partitie(int, vector<int>);
   
    void complete_graph();
   
    void minimize_dfa();
};

void DFA::add_transition(int nod1, int nod2, string s)
{
    Tranzitie* tranzitie = new Tranzitie(nod1, nod2, s);
    this->transitions[nod1].push_back(*tranzitie);
}

void DFA::add_transition(Tranzitie T)
{
    this->transitions[T.get_Nod1()].push_back(T);
}

void DFA::add_state(int nod)
{
    this->states.push_back(nod);
}

void DFA::add_final_state(int nod)
{
    this->final_states.push_back(nod);
}

void DFA::set_initial_state(int nod)
{
    this->initial_state = nod;
}

int DFA::get_initial_state()
{
    return this->initial_state;
}

void DFA::clear_states()
{
    this->states.clear();
}

void DFA::clear_transitions()
{
    this->transitions.clear();
}

int DFA::get_max_state()
{
    int maxim = 0;
    for (int i = 0; i < this->states.size(); ++i)
    {
        if (this->states[i] > maxim)
            maxim = this->states[i];
    }
    return maxim;
}

int DFA::get_specified_state(int number)
{
    for (auto state = this->states.begin(); state != this->states.end(); ++state)
    {
        int aux = *state;
        while (aux)
        {
            if (aux % 10 == number)
                return *state;
            aux /= 10;
        }
    }
}

void DFA::print_states()
{
    for (int i = 0; i < this->states.size(); ++i)
    {
        cout << this->states[i] << ' ';
    }
    cout << endl;
}

void DFA::print_transitions()
{
    for (int i = 0; i < this->transitions.size(); ++i)
    {
        for (int j = 0; j < this->transitions[i].size(); ++j)
        {
            cout << this->transitions[i][j] << '\n'
                ;
        }
    }
}

void DFA::print_final_states()
{
    for (int i = 0; i < this->final_states.size(); ++i)
    {
        cout << this->final_states[i] << ' ';
    }
    cout << endl;
}

vector<int> DFA::get_states()
{
    return this->states;
}

bool DFA::is_final_state(int nod)
{
    for (auto i = this->final_states.begin(); i != this->final_states.end(); ++i)
    {
        if (*i == nod)
        {
            return true;
        }
    }
    return false;
}

bool DFA::mai_exista_partitii()
{   //Toate partitiile:
    for (auto partitie = partitii.begin(); partitie != partitii.end(); ++partitie) //{q0, q1, q2, q3, q4}, {}
    {   //Toate starile din partitie:
        if ((*partitie).size() == 1) continue;

        for (int stare = 0; stare < (*partitie).size() - 1; ++stare) //q0, q1, q2, q3, q4;
        {  
            for (auto stare_avansata = stare + 1; stare_avansata < (*partitie).size(); ++stare_avansata)
            {
                //Pt fiecare litera din alfabet:
                for (int litera = 0; litera < this->alfabet.size(); ++litera)
                {
                    //Cautam tranzitiile celor doua stari care au litera corespunzatoare:
                    int nod1 = -1, nod2 = -1;

                    for (int i = 0; i < this->transitions[(*partitie)[stare]].size(); ++i)
                    {
                        if (this->transitions[(*partitie)[stare]][i].get_litera()[0] == this->alfabet[litera])
                        {
                            nod1 = this->transitions[(*partitie)[stare]][i].get_Nod2();
                            continue;
                        }
                    }

                    for (int i = 0; i < this->transitions[(*partitie)[stare_avansata]].size(); ++i)
                    {
                        if (this->transitions[(*partitie)[stare_avansata]][i].get_litera()[0] == this->alfabet[litera])
                        {
                            nod2 = this->transitions[(*partitie)[stare_avansata]][i].get_Nod2();
                            continue;
                        }
                    }

                    if (nod1 != -1 && nod2 != -1 && !check_if_in_same_partition(nod1, nod2))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool DFA::check_if_in_same_partition(int stare1, int stare2)
{
    if (stare1 == stare2) return true;
    for (auto partitie = partitii.begin(); partitie != partitii.end(); ++partitie)
    {
        int c1 = 0, c2 = 0;
        for (auto stare = (*partitie).begin(); stare != (*partitie).end(); ++stare)
        {
            if (*stare == stare1) c1++;
            else if (*stare == stare2) c2++;
        }

        if (c1 != 0 && c2 != 0)
            return true;
    }
    return false;
}

//----------------------TREBUIE COMPLETAT GRAFUL CU TOATE MUCHIILE-------------------------------------------------------------------------

void DFA :: construieste_partitii_noi()
{
    vector<vector<int>>partitii_temporare;
    //caz special daca partitia are deja un singur element;
    for (auto partitie = this->partitii.begin(); partitie != this->partitii.end(); ++partitie)
    {
        if ((*partitie).size() == 1)
        {
            partitii_temporare.push_back(*partitie);
            continue;
        }
       
        //Altfel parcurg toate starile din partitie fiecare cu fiecare si verific daca pot fi separate:
        else
        {
            for (int stare = 0; stare < (*partitie).size() - 1; ++stare) //q1, q2, q3, q4;
            {
                for (int stare_avansata = stare + 1; stare_avansata < (*partitie).size(); ++stare_avansata)
                {
                    bool se_pot_distinge = false;
                    for (int litera = 0; litera < this->alfabet.size(); ++litera)
                    {
                        //Cautam tranzitiile celor doua stari care au litera corespunzatoare:
                        int nod1 = -1, nod2 = -1;

                        for (int i = 0; i < this->transitions[(*partitie)[stare]].size(); ++i)
                        {
                            if (this->transitions[(*partitie)[stare]][i].get_litera()[0] == this->alfabet[litera])
                            {
                                nod1 = this->transitions[(*partitie)[stare]][i].get_Nod2();
                                continue;
                            }
                        }

                        for (int i = 0; i < this->transitions[(*partitie)[stare_avansata]].size(); ++i)
                        {
                            if (this->transitions[(*partitie)[stare_avansata]][i].get_litera()[0] == this->alfabet[litera])
                            {
                                nod2 = this->transitions[(*partitie)[stare_avansata]][i].get_Nod2();
                                continue;
                            }
                        }
                        //Daca se pot distinge:
                        if (nod1 != -1 && nod2 != -1 && !check_if_in_same_partition(nod1, nod2))
                        {   
                            se_pot_distinge = true;
                            //Caut daca stare sau stare_avansata se potrivesc la o partitie deja formata:
                            bool se_potriveste = false;
                            for (auto partitie_temporara = partitii_temporare.begin(); partitie_temporara != partitii_temporare.end(); ++partitie_temporara)
                            {
                                if (se_potriveste_la_partitie((*partitie)[stare], *partitie_temporara)) //si daca nu se afla deja in partitie:
                                {   
                                    bool se_afla_in_partitie = false;
                                    for (int i = 0; i < (*partitie_temporara).size(); ++i)
                                    {
                                        if ((*partitie_temporara)[i] == (*partitie)[stare])
                                        {
                                            se_afla_in_partitie = true;
                                        }
                                    }
                                    if (!se_afla_in_partitie)
                                    {
                                        (*partitie_temporara).push_back((*partitie)[stare]);
                                    }
                                       se_potriveste = true;
                                }
                            }
                            //Daca stare nu se potriveste la nicio partitie formata pana acum:
                            if (!se_potriveste)
                            {
                                vector<int>partitie_noua;
                                partitie_noua.push_back((*partitie)[stare]);
                                partitii_temporare.push_back(partitie_noua);
                            }

                            se_potriveste = false;

                            for (auto partitie_temporara = partitii_temporare.begin(); partitie_temporara != partitii_temporare.end(); ++partitie_temporara)
                            {
                                if (se_potriveste_la_partitie((*partitie)[stare_avansata], *partitie_temporara))
                                {
                                    bool se_afla_in_partitie = false;
                                    for (int i = 0; i < (*partitie_temporara).size(); ++i)
                                    {
                                        if ((*partitie_temporara)[i] == (*partitie)[stare_avansata])
                                        {
                                            se_afla_in_partitie = true;
                                        }
                                    }
                                    if (!se_afla_in_partitie)
                                    {
                                        (*partitie_temporara).push_back((*partitie)[stare_avansata]);
                                    }
                                    se_potriveste = true;
                                }
                            }
                            //Daca stare nu se potriveste la nicio partitie formata pana acum:
                            if (!se_potriveste)
                            {
                                vector<int>partitie_noua;
                                partitie_noua.push_back((*partitie)[stare_avansata]);
                                partitii_temporare.push_back(partitie_noua);
                            }
                        }

                    }
                    //Daca nu se pot distinge:
                    if (!se_pot_distinge)
                    {
                        bool se_potriveste = false;
                        //Caut daca prima stare se potriveste la vreo partitie de pana acum:
                        for (auto partitie_temporara = partitii_temporare.begin(); partitie_temporara != partitii_temporare.end(); ++partitie_temporara)
                        {
                            if (se_potriveste_la_partitie((*partitie)[stare], *partitie_temporara))
                            {
                                se_potriveste = true;
                                bool prima_stare_se_afla_in_partitie = false, a_doua_stare_se_afla_in_partitie = false;
                                for (int i = 0; i < (*partitie_temporara).size(); ++i)
                                {
                                    if ((*partitie_temporara)[i] == (*partitie)[stare])
                                    {
                                        prima_stare_se_afla_in_partitie = true;
                                    }

                                    if ((*partitie_temporara)[i] == (*partitie)[stare_avansata])
                                    {
                                        a_doua_stare_se_afla_in_partitie = true;
                                    }
                                }
                                if (!prima_stare_se_afla_in_partitie && !a_doua_stare_se_afla_in_partitie)
                                {
                                    (*partitie_temporara).push_back((*partitie)[stare]);
                                    (*partitie_temporara).push_back((*partitie)[stare_avansata]);
                                }
                                else if (!prima_stare_se_afla_in_partitie)
                                {
                                    (*partitie_temporara).push_back((*partitie)[stare]);
                                }
                                else if (!a_doua_stare_se_afla_in_partitie)
                                {
                                    (*partitie_temporara).push_back((*partitie)[stare_avansata]);
                                }
                            }
                        }

                        //Altfel fac o noua partitie cu cele doua:
                        if (!se_potriveste)
                        {
                            vector<int>partitie_noua;
                            partitie_noua.push_back((*partitie)[stare]);
                            partitie_noua.push_back((*partitie)[stare_avansata]);
                            partitii_temporare.push_back(partitie_noua);
                        }
                    }
                }
            }
        }
    }
    this->partitii.clear();
    for (auto partitie = partitii_temporare.begin(); partitie != partitii_temporare.end(); ++partitie)
    {
        this->partitii.push_back(*partitie);
    }

}

bool DFA::se_potriveste_la_partitie(int stare, vector<int>partitie)
{
    //pt fiecare stare din partitie:
    for (auto st = partitie.begin(); st != partitie.end(); ++st)
    {
        //pt fiecare litera:
        for (int litera = 0; litera < this->alfabet.size(); ++litera)
        {
            int nod1 = -1, nod2 = -1;

            for (int i = 0; i < this->transitions[stare].size(); ++i)
            {
                if (this->transitions[stare][i].get_litera()[0] == this->alfabet[litera])
                {
                    nod1 = this->transitions[stare][i].get_Nod2();
                    continue;
                }
            }

            for (int i = 0; i < this->transitions[*st].size(); ++i)
            {
                if (this->transitions[*st][i].get_litera()[0] == this->alfabet[litera])
                {
                    nod2 = this->transitions[*st][i].get_Nod2();
                    continue;
                }
            }
            //Daca nodurile destinatie nu sunt in aceeasi partitie, atunci stare nu se potriveste la partitie:
            if (nod1 != -1 && nod2 != -1 && !check_if_in_same_partition(nod1, nod2))
            {
                return false;
            }

        }
    }
    return true;
}

void DFA::complete_graph()
{
    for (auto stare = this->states.begin(); stare != this->states.end(); ++stare) //Trecem prin fiecare stare
    {//Verificam pentru fiecare litera din alfabet daca exista o tranzitie cu acea litera:
        for (int lit = 0; lit < this->alfabet.size(); ++lit)
        {
            bool exista = false;
            for (auto tranzitie = this->transitions[*stare].begin(); tranzitie != this->transitions[*stare].end(); ++tranzitie)
            {
                if ((*tranzitie).get_litera()[0] == this->alfabet[lit])
                {
                    exista = true;
                }
            }
            if (!exista)
            {
                this->add_transition(*stare, *stare, to_string(this->alfabet[lit] - '0'));
            }
        }
    }
}

void DFA :: minimize_dfa()
{
    this->complete_graph();
    vector<int>f_states, non_f_states;

    for (auto i = this->states.begin(); i != this->states.end(); ++i)
    {
        if (is_final_state(*i))
            f_states.push_back(*i);
        else
            non_f_states.push_back(*i);
    }

    this->partitii.push_back(f_states);
    this->partitii.push_back(non_f_states);
  
    while (this->mai_exista_partitii())
    {
        this->construieste_partitii_noi();
    }

    cout << "Partitiile finale sunt:\n";
    for (auto i = this->partitii.begin(); i != this->partitii.end(); ++i)
    {
        for (auto j = (*i).begin(); j != (*i).end(); ++j)
            cout << *j << ' ';
        cout << endl;
    }

    //Acum nu se mai pot partitiona starile, deci le grupez:
    
    this->clear_states();
    vector<vector<Tranzitie>>tranzitii_finale;

    for (auto partitie = this->partitii.begin(); partitie != this->partitii.end(); ++partitie)
    {
        int stare_compusa = 0;
        for (auto element = (*partitie).begin(); element != (*partitie).end(); ++element)
        {
            stare_compusa = stare_compusa * 10 + (*element);
        }
        this->add_state(stare_compusa);
    }

    for (auto stare = this->states.begin(); stare != this->states.end(); ++stare)
    {
        //Pt fiecare litera din alfabet:
        for (int litera = 0; litera < this->alfabet.size(); ++litera)
        {
            //Caut partitia in care se gaseste nodul destinatie:
            for (auto partitie = this->partitii.begin(); partitie != this->partitii.end(); ++partitie)
            {
                for (auto stare_in_partitie = (*partitie).begin(); stare_in_partitie != (*partitie).end(); ++stare_in_partitie)
                {
                    for(auto tranzitie = this->transitions[(*stare) % 10].begin(); tranzitie != this->transitions[(*stare) % 10].end(); ++tranzitie)
                        if ((*stare_in_partitie) == (*tranzitie).get_Nod2() && this->alfabet[litera] == (*tranzitie).get_litera()[0])
                        {
                            Tranzitie* T = new Tranzitie((*stare), this->get_specified_state(*stare_in_partitie), to_string(this->alfabet[litera] - '0'));
                            vector<Tranzitie>v;
                            v.push_back(*T);
                            tranzitii_finale.push_back(v);
                     }
                }
            }
        }
    }
    
    //Adaugam tranzitiile:
    this->clear_transitions();
    this->transitions.resize(this->get_max_state() + 1);
    for (int i = 0; i < tranzitii_finale.size(); ++i)
    {
        for (auto j = tranzitii_finale[i].begin(); j != tranzitii_finale[i].end(); ++j)
        {
            this->add_transition(*j);
        }
    }
     //Adaugam starile finale:
    vector<int>temp_final_states;
   for (int i = 0; i < this->states.size(); ++i)
    {
        int aux = this->states[i];
        while (aux)
        {  
            if (this->is_final_state(aux % 10))
            {
                temp_final_states.push_back(this->states[i]);
                break;
            }
            aux /= 10;
        }
    }
    this->final_states.clear();
    for (int i = 0; i < temp_final_states.size(); ++i)
    {
        this->add_final_state(temp_final_states[i]);
    }
    //Adaugam starea initiala:
    for (int i = 0; i < this->states.size(); ++i)
    {
        int aux = this->states[i];
        while (aux)
        {
            if (this->get_initial_state() == aux % 10)
            {
                this->set_initial_state(this->states[i]);
                break;
            }
            aux /= 10;
        }
    }

}

int main()
{
    int num_of_states, num_of_transitions, nod_plecare, nod_destinatie, initial_state_index, final_state_index, number_of_final_states, number_of_strings;
    string litera;
    DFA* G;
    fin >> num_of_states >> num_of_transitions;
    G = new DFA(num_of_states, num_of_transitions);
    
    for (int i = 1; i <= num_of_states; ++i)
    {
        G->add_state(i);
    }
    for (int i = 0; i < num_of_transitions; ++i)
    {
        fin >> nod_plecare >> nod_destinatie >> litera;
        //Construiesc alfabetul:
        bool ok = false;
        for (int i = 0; i < G->alfabet.size(); ++i)
            if (G->alfabet[i] == litera[0])
                ok = true;
        if (!ok)
            G->alfabet += litera[0];
      
        G->add_transition(nod_plecare, nod_destinatie, litera);
    }

    fin >> initial_state_index;
    G->set_initial_state(initial_state_index);

    fin >> number_of_final_states;
    for (int i = 0; i < number_of_final_states; ++i)
    {
        fin >> final_state_index;
        G->add_final_state(final_state_index);
    }

    G->minimize_dfa();

    cout << "Noile stari ale automatului sunt:\n";
    G->print_states();
    cout << "Noile tranzitii ale automatului sunt:\n";
    G->print_transitions();
    cout << "Noua stare initiala este:\n";
    cout << G->get_initial_state()<<'\n';
    cout << "Noile stari finale sunt:\n";
    G->print_final_states();
    
    return 0;
}