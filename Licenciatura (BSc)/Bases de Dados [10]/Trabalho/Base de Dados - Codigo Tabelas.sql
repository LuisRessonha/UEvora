Create table motorista ( 
	Nome varchar(20),
	NCartaCond char(15),
	DataNasc timestamp,
	Nbi INTEGER,
	primary key(Nbi)	
);


Create table telefone ( 
    Nbi INTEGER,
	Telefone INTEGER,
	primary key(Nbi,Telefone),
	foreign key(Nbi) references motorista(Nbi)
);


Create table modelo ( 
    Marca varchar(30),
	Modelo varchar(30),
	NLugares INTEGER,
	Consumo char(15),
	primary key(Modelo)	
);


Create table taxi ( 
    Modelo varchar(30),
	Ano INTEGER,
	Kms char(10),
	Matricula varchar(8),
	primary key(Matricula)	
);


Create table servico ( 
    DataInicio timestamp,
	DataFim timestamp,
	Kms char(10),
	Valor char(5),
	Matricula varchar(8),
	CoordGPSInic varchar(15),
	CoordGPSfim varchar(15),
	primary key(Matricula,DataInicio),
	foreign key(Matricula) references taxi(Matricula)
);


Create table turno ( 
    DataInicio timestamp,
	DataFim timestamp,
	KmInicio char(10),
	KmFim char(10),
	Matricula varchar(8),
	Nbi INTEGER,
	primary key(Nbi,DataInicio),
	foreign key(Nbi) references motorista(Nbi)
);


Create table cliente ( 
    Nome varchar(50),
	Morada varchar(50),
	CodigoPostal varchar(30),
	Nif bigint,
	primary key(Nif)	
);


Create table pedido ( 
    Nif bigint,
	MoradaInicio varchar(50),
	CodigoPostalInicio varchar(30),
	DataPedido timestamp,
	Matricula varchar(8),
	DataInicio timestamp,
	primary key(Nif,DataPedido),
	foreign key(Nif) references cliente(Nif)
);