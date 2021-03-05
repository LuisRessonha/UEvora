public class PCB {

	private int id;
	private String states;
	private int tempoInicio;
	private int tempoServico;
	private int[] instruction;
	private int programCounter;
	
	public PCB(int id, String states, int t_i, int[] instructions) {
		this.id = id;
		this.states = states;
		this.tempoInicio = t_i;
		this.setInstructions(instruction);
		this.tempoServico = 0;
		this.programCounter = 0;
	}
	
	//Construtor para um PCB vazio 
	public PCB() {
		this.id = -1;
	}
	
	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getStates() {
		return states;
	}

	public void setStatus(String states) {
		this.states = states;
	}

	public int getTempo_inicio() {
		return tempoInicio;
	}

	public void setTempo_inicio(int tempo_inicio) {
		this.tempoInicio = tempo_inicio;
	}

	public int getTempo_servico() {
		return tempoServico;
	}

	public void setTempo_servico(int tempo_servico) {
		this.tempoServico = tempo_servico;
	}
	
	public int[] getInstrucoes() {
		return instruction;
	}

	public void setInstrucoes(int[] instruction) {
		this.instruction = instruction;
	}
	
	public int getTempoInicio() {
		return tempoInicio;
	}

	public void setTempoInicio(int tempoInicio) {
		this.tempoInicio = tempoInicio;
	}

	public int getTempoServico() {
		return tempoServico;
	}

	public void setTempoServico(int tempoServico) {
		this.tempoServico = tempoServico;
	}

	public int getProgramCounter() {
		return programCounter;
	}

	public void setProgramCounter(int instrucaoActual) {
		this.programCounter = instrucaoActual;
	}
	
	public String toString() {
		String pcbAsString = "";
		pcbAsString = 
				"ID: "+this.id+"\n"+
				"States: "+this.states+"\n"+
				"Tempo Inicio: "+this.tempoInicio+"\n"+
				"Tempo Servico: "+this.tempoServico+"\n";
		
		pcbAsString += "Instrucoes: ";
		
		for (int i = 0; i < this.instruction.length; i++) {
			pcbAsString += this.instruction[i]+" ";
		}
		
		return pcbAsString;

	}
	
}