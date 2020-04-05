#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>

const int min_c = -1, max_c = 1;//коэффициенты сбалансированности

using namespace std;

void stream_cleaning() {
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
}

struct Node {
	int info;
	int coefficient = 0;
	Node* left = nullptr;
	Node* right = nullptr;
};

Node* node_search(Node* current, int value) {
	if (current == nullptr)
		return nullptr;
	if (value > current->info) {
		if (current->right != nullptr)
			current = node_search(current->right, value);
	}
	else if (value < current->info) {
		if (current->left != nullptr)
			current = node_search(current->left, value);
	}
	return current;
}

Node* node_search(Node* current, Node*& previous, int value) {//поиск вместе с предыдущим
	if (current == nullptr)
		return nullptr;
	if (value > current->info) {
		if (current->right != nullptr) {
			previous = current;
			current = node_search(current->right, previous, value);
		}
	}
	else if (value < current->info) {
		if (current->left != nullptr) {
			previous = current;
			current = node_search(current->left, previous, value);
		}
	}
	return current;
}

void tree_direct_walk(Node* root) {
	cout << root->info << " ";
	if (root->left != nullptr)
		tree_direct_walk(root->left);
	if (root->right != nullptr)
		tree_direct_walk(root->right);
}

void tree_reverse_walk(Node* root) {
	if (root->left != nullptr)
		tree_reverse_walk(root->left);
	if (root->right != nullptr)
		tree_reverse_walk(root->right);
	cout << root->info << " ";
}

void tree_symmetrical_walk(Node* root) {
	if (root->left != nullptr)
		tree_symmetrical_walk(root->left);
	cout << root->info << " ";
	if (root->right != nullptr)
		tree_symmetrical_walk(root->right);
}

template<class T>
struct List {
	T info;
	List* next = nullptr;
};

Node* node_search(Node* current, List<Node*>*& head, int value) {//с цепочкой предыдущих
	if (current == nullptr)
		return nullptr;
	List<Node*>* newl;

	if (value > current->info) {
		head->info = current;
		if (current->right != nullptr) {
			newl = new List<Node*>;
			newl->next = head;
			head = newl;
			current = node_search(current->right, head, value);
		}
	}
	else if (value < current->info) {
		head->info = current;
		if (current->left != nullptr) {
			newl = new List<Node*>;
			newl->next = head;
			head = newl;
			current = node_search(current->left, head, value);
		}
	}
	return current;
}

int max_lenght(Node* root) {
	int lenght = 1, left = 0, right = 0;
	if (root->left != nullptr)
		left = max_lenght(root->left);
	if (root->right != nullptr)
		right = max_lenght(root->right);

	if (left >= right)
		lenght += left;
	else
		lenght += right;

	return lenght;
}

void conversion_of_coefficients(Node* root) {
	if (root != nullptr) {
		int max_left = 0, max_right = 0;
		if (root->left != nullptr) {
			max_left = max_lenght(root->left);
			conversion_of_coefficients(root->left);
		}

		if (root->right != nullptr) {
			max_right = max_lenght(root->right);
			conversion_of_coefficients(root->right);
		}

		root->coefficient = max_right - max_left;
	}
}

void tree_turn_right(Node* current, Node*& root) {//малый поворот направо
	//изменить коэффициенты и переписать ссылку предыдущего
	//определить вариант поворота
	Node* previous = nullptr, * temp;
	node_search(root, previous, current->info);//ближайший узел
	if (current->right == nullptr) {
		if (current->left->right == nullptr) {//2 подряд с левой стороны
			temp = current;
			current = current->left;
			temp->left = nullptr;
			current->right = temp;
			if (previous != nullptr) {
				if (current->info > previous->info)
					previous->right = current;
				else
					previous->left = current;
			}
			else
				root = current;
			/*current->coefficient = 0;		//Эти коэффициенты и далее можно раскомментировать и убрать функцию пересчета коэффициентов,
			current->right->coefficient = 0;	//если функция малого поворота используется только при добавлении элементов,
			current->left->coefficient = 0;*/	//если же функция малого поворота используется при больших нарушениях балансировки,
		}						//те. когда коэффициенты выходят за рамки min_c, max_c больше, чем на 1 значение,
		else {// 1 слева и 1 справа левого		//необходимо использовать функцию пересчета коэффициентов
			temp = current;
			current = current->left->right;
			current->left = temp->left;
			temp->left->right = nullptr;
			temp->left = nullptr;
			current->right = temp;
			if (previous != nullptr) {
				if (current->info > previous->info)
					previous->right = current;
				else
					previous->left = current;
			}
			else
				root = current;
			/*current->coefficient = 0;
			current->right->coefficient = 0;
			current->left->coefficient = 0;*/
		}
	}
	else {
		temp = current->left->right;
		current->left->right = current;
		Node* temp2 = current->left;
		current->left = temp;
		current = temp2;
		if (previous != nullptr) {
			if (current->info > previous->info)
				previous->right = current;
			else
				previous->left = current;
		}
		else
			root = current;
		/*current->coefficient = 0;
		current->right->coefficient = 0;*/
	}
	conversion_of_coefficients(root);
}

void tree_turn_left(Node* current, Node*& root) {//малый поворот налево
	//изменить коэффициенты и переписать ссылку предыдущего
	//определить вариант поворота
	Node* previous = nullptr, * temp;
	node_search(root, previous, current->info);//ближайший узел
	if (current->left == nullptr) {
		if (current->right->left == nullptr) {//2 подряд с правой стороны
			temp = current;
			current = current->right;
			temp->right = nullptr;
			current->left = temp;
			if (previous != nullptr) {
				if (current->info > previous->info)
					previous->right = current;
				else
					previous->left = current;
			}
			else
				root = current;
			/*current->coefficient = 0;		//Эти коэффициенты и далее можно раскомментировать и убрать функцию пересчета коэффициентов,
			current->left->coefficient = 0;		//если функция малого поворота используется только при добавлении элементов,
			current->right->coefficient = 0;*/	//если же функция малого поворота используется при больших нарушениях балансировки,
		}						//те. когда коэффициенты выходят за рамки min_c, max_c больше, чем на 1 значение,
		else {// 1 справа и 1 слева правого		//необходимо использовать функцию пересчета коэффициентов
			temp = current;
			current = current->right->left;
			current->right = temp->right;
			temp->right->left = nullptr;
			temp->right = nullptr;
			current->left = temp;
			if (previous != nullptr) {
				if (current->info > previous->info)
					previous->right = current;
				else
					previous->left = current;
			}
			else
				root = current;
			/*current->coefficient = 0;
			current->left->coefficient = 0;
			current->right->coefficient = 0;*/
		}
	}
	else {
		temp = current->right->left;
		current->right->left = current;
		Node* temp2 = current->right;
		current->right = temp;
		current = temp2;
		if (previous != nullptr) {
			if (current->info > previous->info)
				previous->right = current;
			else
				previous->left = current;
		}
		else
			root = current;
		/*current->coefficient = 0;
		current->left->coefficient = 0;*/
	}
	conversion_of_coefficients(root);
}

void big_tree_turn_right(Node* current, Node*& root) {
	Node* previous = nullptr, * temp, * temp2;
	node_search(root, previous, current->info);//ближайший узел

	temp = current->left;//temp - z
	current->left = temp->right->right;//y слева равно C
	temp2 = temp->right;//temp2 - x
	temp->right = temp->right->left;//z справа равно B
	temp2->left = temp;//x слева равно z
	temp2->right = current;//x справа равно y
	current = temp2;
	if (previous != nullptr) {
		if (current->info > previous->info)
			previous->right = current;
		else
			previous->left = current;
	}
	else
		root = current;

	//функция пересчета коэффициентов
	conversion_of_coefficients(root);
}

void big_tree_turn_left(Node* current, Node*& root) {
	Node* previous = nullptr, * temp, * temp2;
	node_search(root, previous, current->info);//ближайший узел

	temp = current->right;
	current->right = temp->left->left;
	temp2 = temp->left;
	temp->left = temp->left->right;
	temp2->right = temp;
	temp2->left = current;
	current = temp2;
	if (previous != nullptr) {
		if (current->info > previous->info)
			previous->right = current;
		else
			previous->left = current;
	}
	else
		root = current;

	//функция пересчета коэффициентов
	conversion_of_coefficients(root);
}

bool add_to_tree(Node*& root, int value, int& N) {
	List<Node*>* head, * temp;
	head = new List<Node*>;
	Node* current = node_search(root, head, value);

	if (current == nullptr) {
		delete head;
		return 0;
	}
	if (value == current->info) {
		do {
			temp = head->next;
			delete head;
			head = temp;
		} while (head != nullptr);
		return 0;
	}
	N++;
	Node* new_g;
	temp = new List<Node*>;
	if (value > current->info) {
		new_g = new Node;
		current->right = new_g;
		new_g->info = value;
		temp->info = new_g;
	}
	if (value < current->info) {
		new_g = new Node;
		current->left = new_g;
		new_g->info = value;
		temp->info = new_g;
	}
	temp->next = head;
	head = temp;
	//пересчет коэффициентов
	do {
		if (head->info->info > head->next->info->info)
			head->next->info->coefficient++;
		else
			head->next->info->coefficient--;

		head = head->next;
	} while (!(head->info->coefficient == 0 or head->info->coefficient > max_c or head->info->coefficient < min_c or head->next == nullptr));
	//остановиться на узле с коэффициентом после изменения 0 или выходящем за пределы допустимых значений, или до конца списка
	//если после этого head не выходит за пределы значений, оставить как есть, иначе балансировка дерева
	//если коэффициент узла head меньше минимального, поворот направо относительно этого узла
	//если коэффициент узла head больше максимального, поворот налево относительно этого узла
	if (head->info->coefficient < min_c) {
		if (head->info->right == nullptr or value <= head->info->left->info)
			tree_turn_right(head->info, root);//малый поворот направо
		else
			big_tree_turn_right(head->info, root);//большой поворот направо
	}
	else if (head->info->coefficient > max_c) {
		if (head->info->left == nullptr or value >= head->info->right->info)
			tree_turn_left(head->info, root);//малый поворот налево
		else
			big_tree_turn_left(head->info, root);//большой поворот налево
	}

	head = temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		delete temp;
	}

	return 1;
}

Node* place_of_balancing(Node* root) {
	Node* place = nullptr;
	if (root->coefficient < min_c or root->coefficient > max_c)
		return root;
	if (root->left != nullptr)
		place = place_of_balancing(root->left);
	if (place != nullptr)
		return place;
	if (root->right != nullptr)
		place = place_of_balancing(root->right);
	return place;
}

void tree_balancing(Node*& root) {
	do {
		if (root == nullptr)
			break;
		conversion_of_coefficients(root);
		Node* place = place_of_balancing(root);
		if (place == nullptr)
			break;

		if (place->coefficient < min_c) {
			int left = 0, right = 0;
			if (place->left != nullptr and place->left->left != nullptr)
				left = max_lenght(place->left->left);
			if (place->left != nullptr and place->left->right != nullptr)
				right = max_lenght(place->left->right);

			if (place->right == nullptr or left >= right)
				tree_turn_right(place, root);//малый поворот направо
			else
				big_tree_turn_right(place, root);//большой поворот направо
		}
		else if (place->coefficient > max_c) {
			int left = 0, right = 0;
			if (place->right != nullptr and place->right->right != nullptr)
				right = max_lenght(place->right->right);
			if (place->right != nullptr and place->right->left != nullptr)
				left = max_lenght(place->right->left);

			if (place->left == nullptr or right >= left)
				tree_turn_left(place, root);//малый поворот налево
			else
				big_tree_turn_left(place, root);//большой поворот налево
		}
	} while (1);
}

bool remove_from_tree(Node*& root, int value) {
	if (root == nullptr)
		return 0;
	Node* previous = nullptr;
	Node* current = node_search(root, previous, value);//ближайший узел
	if (value != current->info)
		return 0;

	if (current->left != nullptr and current->right != nullptr) {//если есть правый и левый, то цепляю правый на правую сторону
		Node* place = current->left;			     //последнего элемента в правой цепочке левого и переписываю предыдущий
		while (place->right != nullptr)
			place = place->right;
		if (previous != nullptr) {
			if (previous->info < current->info)
				previous->right = current->left;
			else
				previous->left = current->left;
		}
		else
			root = current->left;
		place->right = current->right;
		delete current;
	}
	else if (current->left != nullptr) {//иначе просто удаляю узел и ставлю на его место следующий
		if (previous != nullptr) {
			if (previous->info < current->info)
				previous->right = current->left;
			else
				previous->left = current->left;
		}
		else
			root = current->left;
		delete current;
	}
	else if (current->right != nullptr) {
		if (previous != nullptr) {
			if (previous->info < current->info)
				previous->right = current->right;
			else
				previous->left = current->right;
		}
		else
			root = current->right;
		delete current;
	}
	else {
		if (previous != nullptr) {
			if (previous->info < current->info)
				previous->right = nullptr;
			else
				previous->left = nullptr;
		}
		else
			root = nullptr;
		delete current;
	}
	tree_balancing(root);

	return 1;
}

void tree_output(ofstream& file, const Node* root, int N, bool in_file);

void tree_randomly(int N, Node*& root, int range) {
	root = new Node;
	root->info = rand() % range - range / 2;
	for (int i = 1; i < N;)
		add_to_tree(root, rand() % range - range / 2, i);
}

bool tree_with_keyboard(int& N, Node*& root) {
	cout << "Вводите целочисленные элементы бинарного дерева, для конца записи введите любой символ или букву\n";
	int value;
	while (1) {
		if (cin >> value) {
			if (root == nullptr) {
				root = new Node;
				root->info = value;
				N++;
			}
			else
				add_to_tree(root, value, N);
			stream_cleaning();
			/*ofstream in_file;
			tree_output(in_file, root, N + 2, 0);*/
		}
		else {
			stream_cleaning();
			if (N == 0)
				return 0;
			return 1;
		}
	}
}

bool tree_from_file(int& N, Node*& root) {
	cout << "Введите элементы бинарного дерева в файл через пробел\n";
	fstream file("file.txt", ios_base::out);
	file.close();
	system("file.txt");
	chrono::time_point<chrono::high_resolution_clock> start, end;
	double elapsed_seconds;
	start = chrono::high_resolution_clock::now();
	file.open("file.txt", ios_base::in);
	char ch;
	int value;
	while (1) {
		if (file >> value) {
			if (root == nullptr) {
				root = new Node;
				root->info = value;
				N++;
			}
			else
				add_to_tree(root, value, N);
		}
		else if (file >> ch);
		else {
			if (N == 0)
				return 0;
			end = chrono::high_resolution_clock::now();
			elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
			cout << "Дерево создано за " << 0.000000001 * elapsed_seconds << " секунды.\n";
			return 1;
		}
	}
	file.close();
}

void delete_tree(Node* root) {
	if (root->left != nullptr)
		delete_tree(root->left);
	if (root->right != nullptr)
		delete_tree(root->right);
	delete root;
}

char* int_to_char(int i) {
	char* str = new char[15];
	bool minus = 0;
	if (i < 0) {
		i = abs(i);
		minus = 1;
	}
	else if (i == 0) {
		str[0] = (unsigned char)(48);
		str[1] = '\0';
		return str;
	}
	str[0] = '\0';
	while (i > 0) {
		for (int i = 14; i > 0; i--)//сдвиг вправо
			str[i] = str[i - 1];
		str[0] = (unsigned char)(48 + i % 10);
		i = i / 10;
	}
	if (minus) {
		for (int i = 14; i > 0; i--)
			str[i] = str[i - 1];
		str[0] = '-';
	}
	return str;
}

void right_shift_matrix(int** matrix, int i, int j, int N) {//сдвиг на 1 вправо // j становится пустой
	int width = N * 3 + 2;
	for (int q = i; q >= 0; q--) {
		for (int k = width - 1; k > j; k--)
			matrix[q][k] = matrix[q][k - 1];
		if (matrix[q][j] != -999998)//если не горизонтальная линия
			matrix[q][j] = -999999;
	}
}

void tree_output(ofstream& file, const Node* root, int N, bool in_file) {
	if (root != nullptr) {
		int width = N * 3 + 2;
		if (in_file)
			file << "АВЛ-дерево. Минимальный коэффициент " << min_c << ", максимальный " << max_c << ".\n";//строка для однозначного определения кодировки текстовым редактором

		List<const Node*>* head, * current, * new_t;
		head = new List<const Node*>;
		head->info = root;
		int** matrix;//здесь будет находиться дерево графа
		int* entry_points = new int[N / 2];//точки, хранящие положение узлов в матрице для следующего уровня дерева
		int i = 0, j = 0, entry_tail = 1;
		matrix = new int* [N * 2];
		for (int i = 0; i < N * 2; i++) {
			matrix[i] = new int[width];
			for (int j = 0; j < width; j++)
				matrix[i][j] = -999999;//-999999 означает символ ' '
		}
		entry_points[0] = 1;//изначальное положение по x корня дерева

		while (head != nullptr) {
			j = 0;
			current = head;
			while (current != nullptr) {//запись информации в матрицу
				matrix[i][entry_points[j]] = current->info->info;
				if (current->info->left != nullptr) {//добавление левой ветки для узла
					for (int q = 0; q < 3; q++) {//сдвиг всех вышестоящих элементов
						right_shift_matrix(matrix, i + 1, entry_points[j], N);
						for (int q = j + 1; q < entry_tail; q++)//увеличение всех точек справа
							entry_points[q]++;
						matrix[i + 1][entry_points[j]] = -999998;//-999998 означает символ '-'
					}
					entry_tail++;
					for (int i = entry_tail; i > j; i--)	  //entry_points[j] хранит точку узла следующего уровня, поэтому сдвиг вправо
						entry_points[i] = entry_points[i - 1];//для ее дублирования,
					j++;//инкремент j возвращает к текущей точке,
					entry_points[j] += 3;//увеличение текущей точки узла на 3(так как матрица сдвинулась на 3 позиции вправо)
				}

				if (current->info->right != nullptr) {//добавление правой ветки узла
					for (int q = 0; q < 3; q++) {
						right_shift_matrix(matrix, i + 1, entry_points[j] + 1, N);
						for (int q = j + 1; q < entry_tail; q++)
							entry_points[q]++;
						matrix[i + 1][entry_points[j] + 1] = -999998;
					}
					entry_points[j] += 3;//меняю старый узел на новый и перехожу к следующему
					j++;
				}
				else {//если нет правого узла, удаляю точку
					for (int i = j; i < entry_tail; i++)//сдвиг влево переходит к следующей точке
						entry_points[i] = entry_points[i + 1];
					entry_tail--;
				}
				current = current->next;
			}
			current = head;
			while (current != nullptr) {//переход к следующей линии элементов на одинаковом расстоянии от вершины
				bool b = 0;
				if (current->info->right != nullptr) {//добавление правого элемента списка на место перед текущим
					new_t = new List<const Node*>;
					new_t->next = current->next;
					current->next = new_t;
					new_t->info = current->info->right;
					b = 1;
				}
				if (current->info->left != nullptr) {//замена элемента списка
					current->info = current->info->left;
					current = current->next;
				}
				else {//удаление элемента списка
					if (current == head) {
						head = current->next;
						delete current;
						current = head;
					}
					else {
						new_t = current;
						current = head;
						while (current->next != new_t)
							current = current->next;
						current->next = new_t->next;
						delete new_t;
						current = current->next;
					}
				}
				if (b)//пропуск добавленного правого элемента
					current = current->next;
			}
			i += 2;
		}

		for (int q = 0; q < i - 1; q++) {//вывод дерева в файл
			if (q % 2 == 0) {//формирование стрелок к узлам
				for (j = 0; j < width; j++) {
					if (matrix[q][j] > -999998) {
						if (in_file)
							file << "|";
						else
							cout << "|";
						j++;
						while (matrix[q][j] > -999998 and j < width) {
							j++;
							if (in_file)
								file << " ";
							else
								cout << " ";
						}
						j--;
					}
					else if (in_file)
						file << " ";
					else
						cout << " ";
				}
				if (in_file)
					file << endl;
				else
					cout << endl;
				if (in_file) {
					for (j = 0; j < width; j++) {
						if (matrix[q][j] > -999998) {
							file << (unsigned char)149;//в консоле нет этого символа
							j++;
							while (matrix[q][j] > -999998 and j < width) {
								j++;
								file << " ";
							}
							j--;
						}
						else
							file << " ";
					}
					file << endl;
				}
			}
			bool b = 0;//b означает первый символ правой или левой ветки, так как иногда после сдвигов матрицы символы '-' выходят левее узлов
			for (j = 0; j < width; j++) {
				if (!b and matrix[q][j] == -999998 and (matrix[q + 1][j] != -999999 or matrix[q - 1][j - 1] != -999999))
					b = 1;
				if (b and matrix[q][j] == -999999)
					b = 0;
				if (matrix[q][j] == -999999 or matrix[q][j] == -999998 and !b) {
					matrix[q][j] = -999999;
					if (in_file)
						file << " ";
					else
						cout << " ";
				}
				else if (matrix[q][j] == -999998) {//нахожу левую точку левой ветки и правую правой
					if (matrix[q + 1][j] != -999999 and matrix[q][j - 1] == -999999)
						if (in_file)
							file << ",";
						else
							cout << ",";
					else if (matrix[q + 1][j] != -999999)
						if (in_file)
							file << ".";
						else
							cout << ".";
					else if (in_file)
						file << "-";
					else
						cout << "-";
				}
				else {
					if (in_file)
						file << matrix[q][j];
					else
						cout << matrix[q][j];
					char* str = int_to_char(matrix[q][j]);
					j += strlen(str) - 1;//пропускаю n пробелов, n равно длине строки числа - 1
					delete[] str;
				}
			}
			if (in_file)
				file << endl;
			else
				cout << endl;
		}

		for (i = 0; i < N * 2; i++)
			delete[] matrix[i];
		delete[] matrix;
	}
}

int negative_node_search(Node* root) {
	int value = 0;
	if (root->info < 0)
		return root->info;
	if (root->left != nullptr)
		value = negative_node_search(root->left);
	return value;
}

int odd_search(Node*& root) {
	int value = 0;
	if (root->info % 2 != 0)
		return root->info;
	if (root->left != nullptr)
		value = odd_search(root->left);
	if (value != 0)
		return value;
	if (root->right != nullptr)
		value = odd_search(root->right);
	return value;
}

//Удалите все отрицательные числа. Каждый нечетный элемент умножьте
//на случайную величину в диапазоне от -2 до 2. Проанализируйте
//скорость работы АВЛ-дерева.
void part5(Node*& root, int& N) {
	chrono::time_point<chrono::high_resolution_clock> start, end;
	double elapsed_seconds;

	start = chrono::high_resolution_clock::now();
	int value = negative_node_search(root);
	while (value) {
		remove_from_tree(root, value);
		N--;
		value = negative_node_search(root);
	}
	end = chrono::high_resolution_clock::now();
	elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	cout << "Удаление отрицательных элементов за " << 0.000000001 * elapsed_seconds << " секунды.\n";

	start = chrono::high_resolution_clock::now();
	value = odd_search(root);
	while (value) {
		remove_from_tree(root, value);
		N--;
		add_to_tree(root, value * (rand() % 5 - 2), N);
		value = odd_search(root);
	}
	end = chrono::high_resolution_clock::now();
	elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	cout << "Изменение нечетных элементов за " << 0.000000001 * elapsed_seconds << " секунды.\n";
}

int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(0));
	chrono::time_point<chrono::high_resolution_clock> start, end;
	double elapsed_seconds;
	ifstream out_file;
	ofstream in_file;
	int choice, N_node = 0;
	Node* root = nullptr;

	while (1) {

		cout << "1 - Формирование АВЛ-дерева из N элементов\n";
		cout << "2 - Вывод бинарного дерева\n";
		cout << "3 - Вставка, удаление или поиск узла бинарного дерева\n";
		cout << "4 - Обход бинарного дерева\n";
		cout << "5 - Задание по варианту\n";
		cout << "6 - Проверка на сбалансированность\n";
		cin >> choice;
		stream_cleaning();

		if (choice == 1) {
			if (root != nullptr) {
				delete_tree(root);
				root = nullptr;
			}
			N_node = 0;
			cout << " 1 - N с клавиатуры, элементы рандомно\n";
			cout << " 2 - элементы с клавиатуры\n";
			cout << " 3 - элементы с файла\n ";
			cin >> choice;
			stream_cleaning();

			switch (choice) {

			case 1:
				cout << "Введите количество элементов бинарного дерева: ";
				cin >> N_node;
				if (N_node <= 0 or N_node > 310) {
					cout << "Размер дерева должен быть больше 0 и меньше 310.\n";
					break;
				}
				stream_cleaning();
				start = chrono::high_resolution_clock::now();
				tree_randomly(N_node, root, 2000);
				end = chrono::high_resolution_clock::now();
				elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				cout << "Дерево создано за " << 0.000000001 * elapsed_seconds << " секунды.\n" << "Элементы в порядке возрастания:\n";
				tree_symmetrical_walk(root);
				cout << endl;
				break;

			case 2:
				if (!tree_with_keyboard(N_node, root))
					cout << "Не введено ни одного элемента дерева.\n";
				else {
					cout << "Дерево создано, элементы в порядке возрастания:\n";
					tree_symmetrical_walk(root);
					cout << endl;
					cout << "Количество элементов: " << N_node << endl;
				}
				break;

			case 3:
				if (!tree_from_file(N_node, root))
					cout << "Не введено ни одного элемента дерева.\n";
				else {
					cout << "Элементы в порядке возрастания:\n";
					tree_symmetrical_walk(root);
					cout << endl;
					cout << "Количество элементов: " << N_node << endl;
				}
				break;
			}
			choice = 0;
		}

		if (choice == 2 and root != nullptr) {
			cout << " 1 - вывод дерева в файл\n";
			cout << " 2 - вывод в консоль(не рекомендуется для деревьев размера больше 25)\n ";
			cin >> choice;
			stream_cleaning();
			if (choice == 1) {
				in_file.open("tree.txt");
				tree_output(in_file, root, N_node, 1);
				in_file.close();
				system("tree.txt");
			}
			else {
				tree_output(in_file, root, N_node, 0);
				system("Pause");
			}
			choice = 0;
		}
		else if (choice == 2)
			cout << "Сначала создайте бинарное дерево.\n";

		if (choice == 3 and root != nullptr) {
			cout << " 1 - добавление узла\n";
			cout << " 2 - удаление узла\n";
			cout << " 3 - поиск узла \n";
			cin >> choice;
			stream_cleaning();
			if (choice == 1) {
				cout << "Добавить узел со значением: ";
				int value = 0;
				cin >> value;
				stream_cleaning();
				start = chrono::high_resolution_clock::now();
				if (add_to_tree(root, value, N_node)) {
					end = chrono::high_resolution_clock::now();
					elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
					cout << "Узел добавлен за " << 0.000000001 * elapsed_seconds << " секунды.\n";
				}
				else
					cout << "Узел с таким значением уже существует.\n";
			}
			else if (choice == 2) {
				cout << "Удалить узел со значением: ";
				int value = 0;
				cin >> value;
				stream_cleaning();
				start = chrono::high_resolution_clock::now();
				if (remove_from_tree(root, value)) {
					end = chrono::high_resolution_clock::now();
					elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
					cout << "Узел удален за " << 0.000000001 * elapsed_seconds << " секунды.\n";
					N_node--;
				}
				else
					cout << "Узел с значением " << value << " не найден\n";
			}
			else if (choice == 3) {
				cout << "Найти узел со значением: ";
				int value = 0;
				cin >> value;
				stream_cleaning();
				start = chrono::high_resolution_clock::now();
				Node* search = node_search(root, value);
				end = chrono::high_resolution_clock::now();
				elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				if (search->info != value)
					cout << "Узел с таким значением не найден.\n";
				else
					cout << "Узел найден.\n";
				cout << "Поиск " << 0.000000001 * elapsed_seconds << " секунды.\n";
			}
			choice = 0;
		}
		else if (choice == 3)
			cout << "Сначала создайте бинарное дерево.\n";

		if (choice == 4 and root != nullptr) {
			cout << "Прямой обход дерева\n";
			tree_direct_walk(root);
			cout << "\nОбратный обход дерева\n";
			tree_reverse_walk(root);
			cout << "\nСимметричный обход дерева\n";
			tree_symmetrical_walk(root);
			cout << endl;
		}
		else if (choice == 4)
			cout << "Сначала создайте бинарное дерево.\n";

		if (choice == 5 and root != nullptr) {
			cout << "\nУдалите все отрицательные числа. Каждый нечетный элемент умножьте \n"
					"на случайную величину в диапазоне от -2 до 2. Проанализируйте \n"
					"скорость работы АВЛ-дерева.\n\n";
			part5(root, N_node);
		}
		else if (choice == 5)
			cout << "Сначала создайте бинарное дерево.\n";

		if (choice == 6 and root != nullptr) {
			start = chrono::high_resolution_clock::now();
			conversion_of_coefficients(root);
			Node* place = place_of_balancing(root);
			if (place == nullptr)
				cout << "Коэффициенты обновлены, балансировка не требуется\n";
			else {
				tree_balancing(root);
				cout << "Коэффициенты обновлены, выполнена балансировка\n";
			}
			end = chrono::high_resolution_clock::now();
			elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
			cout << "Проверка на сбалансированность " << 0.000000001 * elapsed_seconds << " секунды.\n";
		}
		else if (choice == 6)
			cout << "Сначала создайте бинарное дерево.\n";

		cout << endl;
	}
}
