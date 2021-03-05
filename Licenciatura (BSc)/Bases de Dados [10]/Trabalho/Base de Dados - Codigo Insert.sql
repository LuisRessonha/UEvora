/* 3(a) veiculos */


-- i.

INSERT INTO modelo VALUES ('Renault', 'Renault Espace', 7, '7L/100Kms');
INSERT INTO taxi VALUES ('Renault Espace', 2015, 123098, '22-AA-22');


-- ii.

INSERT INTO modelo VALUES ('Mercedes', 'MERCEDES CLK', 7, '9L/100Kms');
INSERT INTO taxi VALUES ('MERCEDES CLK', 2014, 234554, '21-AA-22');


-- iii.

INSERT INTO modelo VALUES ('Honda', 'Honda Civic', 5, '5L/100Kms');
INSERT INTO taxi VALUES ('Honda Civic', 2012, 89764, '20-AA-22');


-- iv.

INSERT INTO modelo VALUES ('Mercedes', 'mercedes-benz classe S', 5, '6.5L/100Kms');
INSERT INTO taxi VALUES ('mercedes-benz classe S', 2015, 79744, '19-AA-22');


------------------------------------------------------------------------

/* 3(b) motoristas */

i.

INSERT INTO motorista VALUES ('Manuel Duarte', 'L-123', '14/1/76', '1234');
INSERT INTO telefone VALUES ('1234', '266262626');
INSERT INTO telefone VALUES ('1234', '939393939');


ii.

INSERT INTO motorista VALUES 	('Fernando Nobre', 'L-124', '14/1/77', '1235');
INSERT INTO telefone VALUES ('1235', '266262627');
INSERT INTO telefone VALUES ('1235', '939393940');


iii.

INSERT INTO motorista VALUES 	('Anibal Silva', 'L-125', '14/1/78', '1236');
INSERT INTO telefone VALUES ('1236', '266262628');
INSERT INTO telefone VALUES ('1236', '939393941');


iv.

INSERT INTO motorista VALUES 	('Francisco Lopes', 'L-126', '14/1/79', '1237');
INSERT INTO telefone VALUES ('1237', '266262629');


------------------------------------------------------------------------

/* 3(c) clientes */

i.

INSERT INTO cliente VALUES ('José Silva', 'Rua António Silva 23', '7100-434 Évora', 600700800900);


ii.

INSERT INTO cliente VALUES ('Francisco Passos', 'Rua Manuel Passos 12', '7000-131 Évora', 600700800901);


iii.

INSERT INTO cliente VALUES ('Pedro Sousa', 'Rua Joaquim Sousa 21', '7500-313 Évora', 600700800902);


------------------------------------------------------------------------

/* 3(d) pedidos */

i.




------------------------------------------------------------------------

/* 3(e) turnos */

i.

INSERT INTO turno VALUES ('2/1/2016 8:00', '2/1/2016 17:00', 79744, 79944, '19-AA-22', 1234);


ii.

INSERT INTO turno VALUES ('2/1/2016 8:00', '2/1/2016 17:00', 89764, 89964, '20-AA-22', 1235);


iii.

INSERT INTO turno VALUES ('3/1/2016 8:00', '3/1/2016 17:00', 234554, 234954, '21-AA-22', 1236);


iv.

INSERT INTO turno VALUES ('3/1/2016 8:00', '3/1/2016 17:00', 123098, 123498, '22-AA-22', 1237);


------------------------------------------------------------------------

/* 4 servicos registados */

ii.

INSERT INTO servico VALUES ('2/1/2016 8:12', '2/1/2016 8:32', 12, 5.25, '19-AA-22', 0.75, 0.76);


iii.

INSERT INTO servico VALUES ('2/1/2016 8:43', '2/1/2016 8:52', 7, 3.25, '19-AA-22', 0.77, 0.79 );


iv.

INSERT INTO servico VALUES ('2/1/2016 8:53', '2/1/2016 9:59', 98, 53.25, '19-AA-22', 0.80, 0.91);


v.

INSERT INTO servico VALUES ('2/1/2016 10:13', '2/1/2016 10:29', 18, 19.25, '19-AA-22', 1.02, 1.12);


vi.

INSERT INTO servico VALUES ('2/1/2016 11:10', '2/1/2016 11:39', 23, 22.25, '19-AA-22', 1.15, 1.25);


vii.

INSERT INTO servico VALUES ('2/1/2016 12:00', '2/1/2016 13:39', 21, 42.25, '19-AA-22', 1.26, 1.29);


viii.

INSERT INTO servico VALUES ('2/1/2016 15:20', '2/1/2016 15:39', 9, 12.25, '19-AA-22', 1.30, 1.32);
