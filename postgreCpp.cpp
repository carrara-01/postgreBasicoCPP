#include <pqxx/pqxx>
#include <iostream>
#include <string>


    std::string nomeDB, senhaDB;
    std::string razao, cpf, endereco, cidade, uf;
    int idade;
    char opcao;

int main() {


    std::cout << "Digite o nome do banco postgres: ";
    std::cin >> nomeDB;

    std::cout << "Digite a senha para o banco " << nomeDB << ": ";
    std::cin >> senhaDB;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try {
        pqxx::connection SQLConn("dbname=" + nomeDB + " user=postgres password=" + senhaDB + " host=localhost port=####"); // Aqui defina a porta liberada no seu firewall

        if (SQLConn.is_open()) {
            std::cout << "Conectado ao banco: " << SQLConn.dbname() << std::endl;
        } else {
            std::cerr << "Não foi possível conectar ao banco." << std::endl;
            return 1;
        }
        std::cout << "Deseja inserir na tabela? (S/N) \n";
        std::cin >> opcao;

        if(opcao == 'S' || opcao == 's'){
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Razao: ";
        std::getline(std::cin, razao);

        std::cout << "Idade: ";
        std::cin >> idade;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "CPF: ";
        std::getline(std::cin, cpf);

        std::cout << "Endereco: ";
        std::getline(std::cin, endereco);

        std::cout << "Cidade: ";
        std::getline(std::cin, cidade);

        std::cout << "UF: ";
        std::getline(std::cin, uf);

        pqxx::work txn(SQLConn);
        txn.exec_params(
            "INSERT INTO CLIENTES (razao, idade, cpf, endereco, cidade, uf) VALUES ($1, $2, $3, $4, $5, $6)",
            razao, idade, cpf, endereco, cidade, uf);
        txn.commit();
        std::cout << "Cliente inserido com sucesso!" << std::endl;
        } 

        std::cout << "Deseja consulta a tabela? (S/N)";
        std::cin >> opcao;
        if(opcao == 'S' || opcao == 's'){
            pqxx::work txn2(SQLConn);
            pqxx::result res =  txn2.exec("SELECT * FROM CLIENTES");
            for(auto const &row : res){
                razao = row["razao"].as<std::string>();
                idade = row["idade"].as<int>();
                cpf = row["cpf"].as<std::string>();
                endereco = row["endereco"].as<std::string>();
                cidade = row["cidade"].as<std::string>();
                uf = row["uf"].as<std::string>();

                std::cout << "\nRazao-social: " << razao << "\nIdade: " << idade
                << "\nCPF: " << cpf << "\nEndereco: " << endereco << "\nCidade: " << cidade << ", " << uf << '\n';
            }
        }

    } catch (const std::exception &e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
