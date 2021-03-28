// Algorytm Kruskala
// Data: 23.06.2020
// Agnieszka Talik

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct node {
	int w1;		// numer pierwszego wierzcholka
	int w2;	// numer drugiego wierzcholka
	int weight;	// waga krawedzi
	node* next;	// nastepnik
};

void add(node*& H, int x)
{
	node* p = new node;
	p->weight = x;
	p->next = H;
	H = p;
}
void add2(node*& H, int v1, int v2, int w)
{
	node* p = new node;
	p->w1 = v1;
	p->w2 = v2;
	p->weight = w;
	p->next = H;
	H = p;
}

void del(node*& H) {
	node* p = H;

	if (p)
	{
		H = p->next;
		delete p;
	}
}

void show(node* H)
{
	node* p = H;
	while (p != NULL)
	{
		cout << p->w1 << setw(3) << p->w2 << setw(3) << p->weight << endl;
		p = p->next;
	}
}

void complete(int size, node*& H)
{
	fstream czytaj;
	czytaj.open("graf.txt");
	int w;	// waga

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			czytaj >> w;
			if (w != 0)
			{
				node* p = new node;
				p->w1 = i;
				p->w2 = j;
				p->weight = w;
				p->next = H;
				H = p;
			}
		}
	}
}

int edge(int m, node*& H)
{
	node* e = NULL;
	while (H)
	{
		if ((H->w1) < (H->w2))
		{
			node* p = new node;
			p->next = e;
			p->w1 = H->w1;
			p->w2 = H->w2;
			p->weight = H->weight;
			e = p;
			H = H->next;
			m++;
		}
		else
			H = H->next;
	}
	H = e;
	return m;
}

void split(node*& H, node*& H1, node*& H2)
{
	bool s = false;

	add(H1, 0);
	add(H2, 0);
	node* p1 = H1;
	node* p2 = H2;

	while (H)
	{
		if (s)
		{
			p2->next = H;
			p2 = p2->next;
		}
		else
		{
			p1->next = H;
			p1 = p1->next;
		}
		H = H->next;
		s = !s;
	}
	p1->next = p2->next = NULL;
	del(H1);
	del(H2);
}

void merge(node*& H1, node*& H2, node*& H) //laczenie H1 i H2 do H (posortowane)
{
	add(H, 0);
	node* T = H;
	while (H1 && H2)
	{
		if (H1->weight < H2->weight)
		{
			T->next = H1;
			H1 = H1->next;
			T = T->next;
		}
		else
		{
			T->next = H2;
			H2 = H2->next;
			T = T->next;
		}
	}

	if (H1 == NULL)
	{
		T->next = H2;
		H2 = NULL;
	}
	else
	{
		T->next = H1;
		H1 = NULL;
	}
	del(H);
}

void mergeSort(node*& H)
{
	if (H != NULL && H->next != NULL)
	{
		node* H1, * H2;
		split(H, H1, H2);
		mergeSort(H1);
		mergeSort(H2);
		merge(H1, H2, H);
	}
}

struct DSNode
{
	int up, rank; // ojciec i ranga
};

class DSStruct
{
private:
	DSNode* Z;
public:
	DSStruct(int n);
	~DSStruct();
	void MakeSet(int v);
	int FindSet(int v);
	void UnionSets(node*& H);
};

DSStruct::DSStruct(int n)
{
	Z = new DSNode[n];
}

DSStruct::~DSStruct()
{
	delete[] Z;
}

void DSStruct::MakeSet(int v)
{
	Z[v].up = v;
	Z[v].rank = 0;
}

int DSStruct::FindSet(int v)
{
	if (Z[v].up != v)
		Z[v].up = FindSet(Z[v].up);
	return Z[v].up; // Zwraca indeks reprezentanta zbioru, w którym jest wierzcho³ek v
}

void DSStruct::UnionSets(node *& H)
{
	int ru, rv; // wierzcholki

	ru = FindSet(H->w1);
	rv = FindSet(H->w2);
	if (ru != rv)
	{
		if (Z[ru].rank > Z[rv].rank)
			Z[rv].up = ru;
		else
		{
			Z[ru].up = rv;
				if (Z[ru].rank == Z[rv].rank)
					Z[rv].rank++;
		}
	}
}

void minWeight(node*& H)
{
	int w = 0;
	node* p = H;
	while (p)
	{
		w = w + p->weight;
		p = p->next;
	}
	cout << endl << "Suma wag MDR: " << w << endl;
}

int main()
{
	int n = 8; // Liczba wierzcho³ków
	int m; // Liczba krawêdzi
	
	node * H = NULL;
	complete(n, H);
	cout << endl << "Lista sasiedztwa: " << endl;
	show(H);

	m = edge(0, H);
	cout << endl << "Lista krawedzi: " << endl;
	show(H);

	cout << endl << "Liczba krawedzi: " << m << endl;

	cout << endl << endl << "########## MergeSort ##########" << endl;
	mergeSort(H);
	cout << endl << "Lista krawedzi: " << endl;
	show(H);
	
	cout << endl << endl << "########## Algorytm Kruskala ##########" << endl;

	DSStruct Z(n); // Struktura zbiorów roz³¹cznych

	for (int i = 0; i < n; i++)
		Z.MakeSet(i);       // Tworzymy osobny zbiór dla kazdego wierzcholka

	node* T = NULL;
	node* p = new node;
	for (int i = 1; i < n; i++) // Pêtla wykonuje siê n - 1 razy
	{
		do
		{
			p = H;
			H = H->next;

		} while (Z.FindSet(p->w1) == Z.FindSet(p->w2));

		Z.UnionSets(p);     // Zbiory z wierzcho³kami ³¹czymy ze sob¹
		add2(T, p->w1, p->w2, p->weight); // Dodajemy krawêdŸ do drzewa

	}

	cout << endl << "Lista krawedzi MDR:" << endl;
	show(T);

	minWeight(T);

	return 0;
}