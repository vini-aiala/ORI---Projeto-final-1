1. Inserir uma nova carona
    Cada carona dever´a ser inserida no final do arquivo de dados e atualizada nos ´ındices.
2. Remover uma carona a partir da chave primária
    o registro dever´a ser localizado acessando o ´ındice prim´ario. A remo¸c˜ao dever´a
    colocar os caracteres *| nas primeiras posi¸c˜oes do registro removido. O espa¸co do registro
    removido n˜ao dever´a ser reutilizado para novas inser¸c˜oes. Observe que o registro dever´a continuar ocupando exatamente 256 bytes. Al´em disso, no ´ındice prim´ario, o RRN correspondente
    ao registro removido dever´a ser substitu´ıdo por -1.
3. Modificar o campo vagas de uma carona a partir da chave primária;
    o ´unico campo alter´avel ´e o de Vagas. O registro dever´a ser localizado acessando
    o ´ındice prim´ario e a quantidade de vagas dever´a ser atualizada no registro na mesma posi¸c˜ao
    em que est´a (n˜ao deve ser feita remo¸c˜ao seguida de inser¸c˜ao). Note que o campo de Vagas
    sempre ter´a 1 d´ıgitos.
4. Buscar caronas a partir:
    1) da chave primária;
    2) da data da viagem;
    3) de uma localidade; ou
    4) de uma localidade em uma data.
5. Listar todas as caronas ordenadas por:
    1) código;
    2) trajeto e chave primária;
    3) nome do motorista e chave primária;
    4) data e hora da viagem; ou
    5) trajeto e data/hora.
6. Liberar espa¸co.
7. Imprimir arquivo de dados.
8. Imprimir arquivo de índices secundários:
    1) Índice secundário para os nomes dos motoristas;
    2) Índice secundário para os trajetos;
    3) Índice secundário para as datas;
    4) Índice secundário para os horários.
9. Finalizar (liberar estruturas da memória RAM).

O arquivo de dados deve ser ASCII (arquivo texto), organizado em registros de tamanho fixo
de 256 bytes (256 caracteres). Os campos nome do motorista, modelo do carro e trajeto devem ser
de tamanho vari´avel. Os demais campos devem ser de tamanho fixo: c´odigo (10 bytes), gˆenero (1
byte), data de nascimento (10 bytes), celular (15 bytes), placa do ve´ıculo (8 bytes), data (8 bytes) e
hora (5 bytes) da carona, valor (6 bytes) e por fim vagas dispon´ıveis (1 byte). A soma de bytes dos
campos fornecidos (incluindo os delimitadores necess´arios) nunca poder´a ultrapassar 256 bytes. Os
campos do registro devem ser separados pelo caractere delimitador @ (arroba). Cada registro ter´a 12
delimitadores, mais 64 bytes ocupados pelos campos de tamanho fixo. Vocˆe precisar´a garantir que os
demais campos juntos ocupem um m´aximo de 180 bytes. Caso o registro tenha menos de 256 bytes,
o espa¸co adicional deve ser marcado com o caractere ] de forma a completar os 256 bytes. Para evitar
que o registro exceda 256 bytes, os campos vari´aveis devem ocupar no m´aximo 180 bytes.