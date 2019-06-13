#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <conio.h>

using namespace std;

const int S_len = 41;
// 0  ... 9    A  ...  Z	a  ...  z
// 48 ... 57   65 ...  90	97      122
const int Add_to_num = 48;
const int Add_to_Alpha = 55;
const int Add_to_alpha = 87;


char num_to_char(int num);
string char_from_dem_int(unsigned long long val, int final_ss);
unsigned long long char_to_dem_int(string str, int initial_ss);

struct div_item
{
	string div = "";
	string minus = "";
	string mod = "";
};

int main()
{
	setlocale(LC_ALL, "Russian");
	
	while (1)
	{

		//ВВОД
		string number_int = "";
		string number_real = "";
		int ss_first = 0, ss_second = 0;
		bool is_real = false;
		int int_len = 0, real_len = 0;
		bool is_error = false;
		do
		{
			char number[S_len];
			cout << "Число ИсходнаяCC КонечнаяCC:\n";
			do
			{
				is_error = false;
				while (!(cin >> number >> ss_first >> ss_second))
				{
					cin.clear();
					while (cin.get() != '\n');
					cout << "Неправильный ввод! Введите снова:\n\n";
					cout << "Число ИсходнаяCC КонечнаяCC:\n";
				}
				if (ss_first < 2 || ss_first > 36 || ss_second < 2 || ss_second > 36)
				{
					is_error = true;
					cout << "Системы счисления должы лежать в диапазоне от 2 до 36! Введите снова:\n\n";
					cout << "Число ИсходнаяCC КонечнаяCC:\n";
				}

			} while (is_error);
			is_error = false;

			int_len = 0;
			real_len = 0;
			is_real = false;
			int temp_len = 0;
			int point_count = 0;
			for (int i = 0; i < S_len; i++)
			{
				if (number[i] == '\0')
				{
					if (is_real) real_len = temp_len;
					else int_len = temp_len;
					break;
				}
				else if (number[i] == '.' || number[i] == ',')
				{
					if (point_count == 0)
					{
						point_count++;
						is_real = true;
						int_len = temp_len;
						temp_len = 0;
					}
					else
					{
						is_error = true;
						break;
					}
				}
				else
				{
					temp_len++;
					if (number[i] < '0' || (number[i] > '9' && number[i] < 'A') || (number[i] > 'Z' && number[i] < 'a') || number[i] > 'z')
					{
						is_error = true;
						break;
					}
					if ((ss_first <= 10 && number[i] >= ss_first + Add_to_num) || (ss_first <= 36 && ((number[i] >= ss_first + Add_to_Alpha && number[i] < 'a') || (number[i] >= ss_first + Add_to_alpha))))
					{
						is_error = true;
						break;
					}
				}
			}

			//если нет ошибок распределяем ввод между number_int и number_real
			if (!is_error)
			{
				for (int i = 0; i < int_len; i++)
					number_int.push_back(number[i]);
				if (is_real)
				{
					is_real = false;
					for (int i = int_len + 1; i < int_len + 1 + real_len; i++)
					{
						if (number[i] != '0')
						{
							is_real = true;
							break;
						}
					}
				}
				if (is_real)
				{
					for (int i = int_len + 1; i < int_len + 1 + real_len; i++)
						number_real.push_back(number[i]);
				}
			}
			else
			{
				cout << "Неверно введено число! Введите снова:\n\n";
			}
		} while (is_error);


		


		//ПЕРЕВОДИМ ЦЕЛУЮ ЧАСТЬ МЕТОДОМ ДЕЛЕНИЯ
		//здесь будет результат
		string final_int = "";

		//вектор с логами деления
		vector< vector<div_item> > divisions(0);

		//вектор для логирования делимых
		vector<string> nums(0);

		//записываем сюда остатки
		vector<string> mods(0);

		//создаём current_num как копию number_int. Здесь будет храниться текущее делимое
		string current_num = number_int;

		//текущее делимое в десятичой системе
		unsigned long long current_num_int = char_to_dem_int(current_num, ss_first);

		//текущее частное в десятичной системе
		unsigned long long answer = current_num_int / ss_second;

		//выполняем цикл, пока текущее частное не равно нулю
		while (answer)
		{
			//логируем текущее делимое
			nums.push_back(current_num);

			//создаём вектор который будем впихивать сзади в divisions
			vector<div_item> temp_division(0);

			//логируем текущее деление
			int dig_count = current_num.size();		//всего разрядов в делимом
			int current_dig = 0;					//текущий обрабатываемый разряд		 
			string cur_div = "0";					//текущее делимое
			string new_div = "";					//часть текущего делимого или оно целиком

			while (current_dig < dig_count)
			{
				if (cur_div[0] == '0')
				{
					//Если получился нулевой остаток, просто сносим цифру
					new_div = "";
					new_div += current_num[current_dig];
				}
				else
				{
					//сносим цифру
					new_div = cur_div;
					new_div += current_num[current_dig];
				}

				//переводим new_div в десятичную и считаем текущую цифру ответа
				unsigned long long new_div_int = char_to_dem_int(new_div, ss_first);
				unsigned long long new_answer = new_div_int / ss_second;

				//если ответ не нулевой
				if (new_answer)
				{
					//логи :)
					div_item temp;
					temp.div = new_div;
					temp.minus = char_from_dem_int(ss_second * new_answer, ss_first);
					temp.mod = char_from_dem_int(new_div_int % ss_second, ss_first);
					temp_division.push_back(temp);

					//перезаписываем остаток в cur_div
					cur_div = temp.mod;

				}
				else
				{
					//обновляем cur_div
					cur_div = new_div;
				}

				current_dig++;
			}

			divisions.push_back(temp_division);

			//пишем остаток
			unsigned long long mod = current_num_int - (answer * ss_second);	//вычисляем остаток
			string mod_char = char_from_dem_int(mod, ss_first);					//переводим в первичную сс
			mods.push_back(mod_char);											//добавляем к вектору
			divisions.back().back().mod = mod_char;

			//обновляем current_num, current_num_int, answer, answer_char
			string answer_char = char_from_dem_int(answer, ss_first);
			current_num = answer_char;
			current_num_int = char_to_dem_int(current_num, ss_first);
			answer = current_num_int / ss_second;
		}

		//добавляем результат последнего деления как остаток
		mods.push_back(current_num);

		//логируем последнее (псевдо) делимое
		nums.push_back(current_num);

		//заполняем final_int остатками задом наперёд
		for (int i = mods.size() - 1; i >= 0; i--)
		{
			string mod_final = char_from_dem_int(char_to_dem_int(mods[i], ss_first), ss_second);
			final_int += mod_final[0];
		}



		//ПЕРЕВОДИМ ДРОБНУЮ ЧАСТЬ МЕТОДОМ УМНОЖЕНИЯ
		//переводим основание в первичную сс
		string calc_base = char_from_dem_int(ss_second, ss_first);

		//Здесь будет ответ
		string final_real = "";

		//Здесь будет вещественная часть множителя
		vector<string> factor_real(1);
		factor_real[0] = number_real;

		//Здесь будет целая часть, которая записывется в остаток
		vector<string> factor_whole(1);
		factor_whole[0] = "0";

		vector<string> whole_final(1);
		whole_final[0] = "0";

		//Здесь будут слагаеммые для суммы
		vector< vector<string> > pluses(0);

		//количество слагаемых
		int pluses_count = calc_base.size();

		//количество блоков умножения
		int blocks_count = 0;

		//булин периодичности
		bool is_repeat = false;

		//Здесь будет индекс, на котором начинается период
		int repeat_index = 0;

		//точность
		int prec = 9;

		//булин переполнения точности
		bool is_overflow = false;

		if (is_real)
		{
			unsigned long long current_factor_dem = char_to_dem_int(factor_real[0], ss_first);

			//пока множитель не равен нулю или не найден период или не переполнена точность производим умножение
			while (current_factor_dem && !is_overflow)
			{
				//пишем слагаемые
				vector<string> pluses_temp(0);
				pluses.push_back(pluses_temp);
				for (int i = 0; i < pluses_count; i++)
				{
					//переводим в десятичную систему очередную цифру основания
					string cur_num = "";
					cur_num += calc_base[pluses_count - 1 - i];
					unsigned long long cur_num_dem = char_to_dem_int(cur_num, ss_first);

					//получаем очередное слагаемое
					unsigned long long answer_dem = current_factor_dem * cur_num_dem;
					string answer_char = "";
					if (answer_dem == 0)
					{
						for (int z = 0; z < real_len; z++)
							answer_char += '0';
					}
					else
					{
						answer_char = char_from_dem_int(answer_dem, ss_first);
						for (int z = 0; z < i; z++)	//приписываем нолики
							answer_char += '0';
					}

					//пихаем к собратьям
					pluses[blocks_count].push_back(answer_char);
				}

				//вычисляем результат очередного блока и обновляем factor
				unsigned long long cur_result_dem = current_factor_dem * ss_second;
				string cur_result = char_from_dem_int(cur_result_dem, ss_first);

				//целая часть результата
				int res_whole_len = cur_result.size() - real_len;
				if (res_whole_len == 0)
				{
					string res_whole = "0";
					factor_whole.push_back(res_whole);
				}
				else
				{
					string res_whole = "";
					for (int i = 0; i < res_whole_len; i++)
					{
						res_whole += cur_result[i];
					}
					factor_whole.push_back(res_whole);
				}


				//вещественная часть результата
				string res_real = "";
				for (int i = res_whole_len; i < cur_result.size(); i++)
				{
					res_real += cur_result[i];
				}
				factor_real.push_back(res_real);

				blocks_count++;

				//проверяем переодичность
				for (int i = 0; i < blocks_count; i++)
				{
					bool is_equal = true;
					if (factor_real.back() != factor_real[i])
						is_equal = false;
					if (is_equal)
					{
						is_repeat = true;
						repeat_index = i;
						break;
					}
				}

				//выход из цикла при периодичности
				if (is_repeat)
					break;

				//выход из цикла на следующей итерации при переполнении точности
				else if (blocks_count >= prec)
					is_overflow = true;

				current_factor_dem = char_to_dem_int(factor_real[blocks_count], ss_first);
			}

			//считаем ответ, преписываем точку
			final_real += '.';
			for (int i = 1; i < factor_whole.size(); i++)
			{
				string final_answer_num = char_from_dem_int(char_to_dem_int(factor_whole[i], ss_first), ss_second);
				final_real += final_answer_num[0];
				whole_final.push_back(final_answer_num);
			}

			//Дописываем троеточие при переполнении
			if (is_overflow)
				final_real += "...";

			//Дописываем скобочки в случае периода
			if (is_repeat)
			{
				string c = "(";
				final_real.insert(repeat_index + 1, c);
				final_real += ')';
			}
		}



		//РЕЗУЛЬТАТЫ	
		//строки, которые будет выводить cout
		vector<string> out_strs(0);

		//ОБРАБАТЫВАЕМ ДЕЛЕНИЕ
		//счётчик обработанных делений
		int divs = 0;

		//цикл, который обрабатывает все деления divisions
		while (divs < divisions.size())
		{
			//строки текущего деления
			vector<string> cur_strs(0);

			//ширина текущего деления
			int cur_num_width = nums[divs].size() + 1;	//+ 1 чтобы поместилась скобочка остатка
			int cur_div_width = (cur_num_width > 6 ? cur_num_width : 6);

			//текущий div_item
			int cur_div = 0;

			//обрабатываем текущий div_item
			while (cur_div < divisions[divs].size())
			{
				//3 строки деления
				string temp_div(cur_div_width + 1, ' ');	// + 1 для последующей палочки деления
				string temp_minus(cur_div_width + 1, ' ');
				string temp_mod(cur_div_width + 1, ' ');

				int temp_div_index = 0;

				//формируем temp_div
				if (cur_div == 0)
				{
					//если это первый div_item, то temp_div равно текущему делимому
					temp_div = nums[divs];
					while (temp_div.size() < cur_div_width + 1) //заполняем пробелами до строго определённого конца строки
						temp_div += ' ';
				}
				else
				{
					//иначе сопоставляем temp_div c предыдущим остатком
					temp_div = cur_strs[cur_div * 2];

					//индекс temp_div
					temp_div_index = temp_div.size() - 1;
					while (temp_div[temp_div_index - 1] == ' ')
						temp_div_index--;

					//индекс текущего div_item.div
					int div_item_div_index = 0;

					//если остаток нулевой, стираем ноль
					if (divisions[divs][cur_div - 1].mod[0] == '0')
						temp_div[temp_div_index - 1] = ' ';
					else
						while (temp_div_index && temp_div[temp_div_index - 1] != ' ')
							temp_div_index--;

					//записываем текущий div_item.div в temp_div
					for (int i = 0; i < divisions[divs][cur_div].div.size(); i++)
					{
						temp_div[temp_div_index] = divisions[divs][cur_div].div[i];
						temp_div_index++;
					}
				}

				//формируем temp_minus и temp_mod
				int minus_index;
				if (cur_div == 0)
					minus_index = divisions[divs][cur_div].div.size() - 1;
				else
					minus_index = --temp_div_index;

				int mod_index;
				if (cur_div == divisions[divs].size() - 1)
					mod_index = cur_num_width - 2;
				else
					mod_index = minus_index;

				for (int i = divisions[divs][cur_div].minus.size() - 1; i >= 0; i--)
				{
					temp_minus[minus_index] = divisions[divs][cur_div].minus[i];
					minus_index--;
				}

				for (int i = divisions[divs][cur_div].mod.size() - 1; i >= 0; i--)
				{
					temp_mod[mod_index] = divisions[divs][cur_div].mod[i];
					mod_index--;
				}

				//пихаем строчки в cur_strs
				if (cur_div != 0)
					cur_strs.pop_back();

				cur_strs.push_back(temp_div);
				cur_strs.push_back(temp_minus);
				cur_strs.push_back(temp_mod);

				//берём остаток в скобочки
				if (cur_div == divisions[divs].size() - 1)
				{
					string mod_final = char_from_dem_int(char_to_dem_int(mods[divs], ss_first), ss_second);
					string temp_mod_dem = "";
					int temp_mod_dem_len = cur_num_width - 2 - int(mod_final.size());
					while (int(temp_mod_dem.size()) < temp_mod_dem_len)
						temp_mod_dem += ' ';
					temp_mod_dem += '[';
					for (int i = 0; i < mod_final.size(); i++)
						temp_mod_dem += mod_final[i];
					temp_mod_dem += ']';
					cur_strs.push_back(temp_mod_dem);
				}

				cur_div++;
			}

			//рисуем палочки
			cur_strs[0][cur_div_width] = '|';
			cur_strs[1][cur_div_width] = '|';
			cur_strs[2][cur_div_width] = '|';

			//пишем основание
			cur_strs[0] += calc_base;

			//рисуем линию
			//ширина следующего деления
			int next_num_width = nums[divs + 1].size() + 1;	//+ 1 чтобы поместилась скобочка остатка
			int next_div_width = (next_num_width > 6 ? next_num_width : 6);

			for (int i = 0; i < next_div_width; i++)
				cur_strs[1] += '-';

			//пишем результат
			cur_strs[2] += nums[divs + 1];

			//берём последний остаток в скобочки
			if (divs == divisions.size() - 1)
			{
				string last_answer_final = char_from_dem_int(char_to_dem_int(nums.back(), ss_first), ss_second);

				while (cur_strs[3].size() < cur_div_width + 1)
					cur_strs[3] += ' ';
				cur_strs[3].pop_back();
				while (cur_strs[3].size() < cur_strs[2].size() + 1 - last_answer_final.size() - 2)
					cur_strs[3] += ' ';
				cur_strs[3] += '[';
				for (int i = 0; i < last_answer_final.size(); i++)
					cur_strs[3] += last_answer_final[i];
				cur_strs[3] += ']';
			}

			//пихаем cur_strs в out_strs;
			if (divs == 0)
			{
				for (int i = 0; i < cur_strs.size(); i++)
					out_strs.push_back(cur_strs[i]);
			}
			else
			{
				int out_str_number = divs * 2;
				int out_str_index = out_strs[out_str_number].size() - nums[divs].size();
				//int first_str_index = out_str_index;

				for (int i = 0; i < nums[divs].size(); i++)
					out_strs[out_str_number].pop_back();

				for (int i = 0; i < cur_strs.size(); i++)
				{
					if (out_str_number < out_strs.size())
					{
						while (out_strs[out_str_number].size() < out_str_index)
							out_strs[out_str_number] += ' ';
						out_strs[out_str_number] += cur_strs[i];
					}
					else
					{
						string temp_str(out_str_index, ' ');
						temp_str += cur_strs[i];
						out_strs.push_back(temp_str);
					}
					out_str_number++;
				}
			}

			divs++;
		}


		//ОБРАБАТЫВАЕМ УМНОЖЕНИЕ
		if (is_real)
		{
			vector<string> real_out(0);

			int max_whole_size = 1;
			for (int i = 1; i < factor_whole.size(); i++)
			{
				if (factor_whole[i].size() > max_whole_size)
					max_whole_size = factor_whole[i].size();
			}

			//длина основания
			int calc_base_len = calc_base.size();

			//длина строк вывода
			int real_out_len = 4 + max_whole_size + 1 + (real_len > calc_base_len + 1 ? real_len : calc_base_len + 1);

			for (int fact = 0; fact < blocks_count; fact++)
			{
				//пихаем в real_out множитель
				string factor_str = "";

				//добавляем пробелы по разнице между максимальной длинной целого и текущей длиной целого
				for (int p = 0; p < max_whole_size - factor_whole[fact].size(); p++)
					factor_str += ' ';

				//добавляем перевод целой части и скобочки
				if (fact == 0)
				{
					for (int i = 0; i < 4; i++)
						factor_str += ' ';
				}
				else
				{
					factor_str += '[';
					factor_str += whole_final[fact];
					factor_str += ']';
					factor_str += '=';
				}

				//добавляем целую часть множителя
				for (int i = 0; i < factor_whole[fact].size(); i++)
					factor_str += factor_whole[fact][i];

				//добавляем палочку
				factor_str += '|';

				//добавляем пробелы если длина основания больше
				for (int p = 0; p < calc_base_len + 1 - real_len; p++)
					factor_str += ' ';

				//добавляем вещественную часть множителя
				for (int i = 0; i < real_len; i++)
					factor_str += factor_real[fact][i];

				real_out.push_back(factor_str);


				//пихаем в real_out основание
				string calc_base_str = "";

				//добавляем пробелы
				for (int p = 0; p < max_whole_size + 4; p++)
					calc_base_str += ' ';

				//добавляем палочку
				calc_base_str += '|';

				//добавляем пробелы если длина целой части больше
				for (int p = 0; p < real_len - calc_base_len - 1; p++)
					calc_base_str += ' ';

				//добавляем знак умножения
				calc_base_str += '*';

				//добавляем основание
				for (int i = 0; i < calc_base_len; i++)
					calc_base_str += calc_base[i];

				real_out.push_back(calc_base_str);


				//пихаем в real_out слагаемые
				for (int pl = 0; pl < pluses_count; pl++)
				{
					bool is_zero = true;
					for (int i = 0; i < pluses[fact][pl].size(); i++)
					{
						if (pluses[fact][pl][i] != '0')
						{
							is_zero = false;
							break;
						}
					}
					if (is_zero)
						continue;

					string temp_plus_str = "";

					//добавляем пробелы по разности между длиной строки перед палочкой и длиной целой части слагаемого
					for (int p = 0; p < 4 + max_whole_size - pluses[fact][pl].size() + real_len; p++)
						temp_plus_str += ' ';

					//добавляем целую часть слагаемого
					for (int i = 0; i < pluses[fact][pl].size() - real_len; i++)
						temp_plus_str += pluses[fact][pl][i];

					//добавляем палочку
					temp_plus_str += '|';

					//добавляем пробелы если длина основания больше
					for (int p = 0; p < calc_base_len + 1 - real_len; p++)
						temp_plus_str += ' ';

					//добавляем вещественную часть слагаемого
					for (int i = pluses[fact][pl].size() - real_len; i < pluses[fact][pl].size() - pl; i++)
						temp_plus_str += pluses[fact][pl][i];

					//заменяем последние нолики пробелами
					for (int i = 0; i < pl; i++)
						temp_plus_str += ' ';

					real_out.push_back(temp_plus_str);
				}


				//пихаем в real_out линию
				string line_str = "";
				for (int l = 0; l < real_out_len; l++)
					line_str += '-';
				real_out.push_back(line_str);
			}

			//пихаем в real_out линию последний множитель
			string factor_str = "";

			//добавляем пробелы по разнице между максимальной длинной целого и текущей длиной целого
			for (int p = 0; p < max_whole_size - factor_whole[blocks_count].size(); p++)
				factor_str += ' ';

			//добавляем перевод целой части и скобочки

			factor_str += '[';
			factor_str += whole_final[blocks_count];
			factor_str += ']';
			factor_str += '=';

			//добавляем целую часть множителя
			for (int i = 0; i < factor_whole[blocks_count].size(); i++)
				factor_str += factor_whole[blocks_count][i];

			//добавляем палочку
			factor_str += '|';

			//добавляем пробелы если длина основания больше
			for (int p = 0; p < calc_base_len + 1 - real_len; p++)
				factor_str += ' ';

			//добавляем вещественную часть множителя
			for (int i = 0; i < real_len; i++)
				factor_str += factor_real[blocks_count][i];

			real_out.push_back(factor_str);


			//добавляем real_out к out_strs
			int max_out_str_len = 0;
			for (int i = 0; i < out_strs.size(); i++)
			{
				if (out_strs[i].size() > max_out_str_len)
					max_out_str_len = out_strs[i].size();
			}

			int real_out_space = 6;

			for (int i = 0; i < real_out.size(); i++)
			{
				if (i < out_strs.size())
				{
					while (out_strs[i].size() < max_out_str_len + real_out_space)
						out_strs[i] += ' ';
					out_strs[i] += real_out[i];
				}
				else
				{
					string temp_str = "";
					while (temp_str.size() < max_out_str_len + real_out_space)
						temp_str += ' ';
					temp_str += real_out[i];
					out_strs.push_back(temp_str);
				}
			}
		}


		//ВЫВОД
		cout << endl;
		for (int i = 0; i < out_strs.size(); i++)
			cout << out_strs[i] << endl;
		cout << "\nОтвет: " << final_int << final_real << endl << endl << endl;
	}

	cout << "Для выхода нажмите любую клавишу ..." << endl;
	_getch();
	return 0;
}



string char_from_dem_int(unsigned long long val, int final_ss)
{
	vector<char> new_str(0);
	while (val >= final_ss)
	{
		int digit = val % final_ss;
		if (digit < 10) digit += Add_to_num;
		else digit += Add_to_Alpha;
		new_str.push_back(char(digit));
		val /= final_ss;
	}
	int digit = val % final_ss;
	if (digit < 10) digit += Add_to_num;
	else digit += Add_to_Alpha;
	new_str.push_back(char(digit));

	string result = "";
	for (int i = new_str.size() - 1; i >= 0; i--)
		result += new_str[i];

	return result;
}



unsigned long long char_to_dem_int(string str, int initial_ss)
{
	unsigned long long result = 0;
	unsigned long long power = 1;
	for (int i = str.size() - 1; i >= 0; i--)
	{
		int digit = str[i];
		if (digit <= 57) digit -= Add_to_num;
		else if (digit <= 90) digit -= Add_to_Alpha;
		else digit -= Add_to_alpha;
		result += digit * power;
		power *= initial_ss;
	}

	return result;
}



char num_to_char(int num)
{
	if (num < 10) num += Add_to_num;
	else num += Add_to_Alpha;
	return char(num);
}
