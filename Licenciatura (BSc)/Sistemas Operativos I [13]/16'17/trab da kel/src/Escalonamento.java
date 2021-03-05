package SO;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;

public class Escalonamento {

	//Filas dos processos
	private ArrayList<PCB> readProc;
	private ArrayList<PCB> newQueue;
	private ArrayList<PCB> readyQueue;
	private ArrayList<PCB> run;
	private ArrayList<PCB> blockedQueue;
	private ArrayList<PCB> exitQueue;
	private ArrayList<PCB> currentStates;

	private int time;
	private int timeOut;
	private int totalProc;
	private boolean useDisk;

	public Escalonamento(String file, int t) {
		this.timeOut = t;
		initVariaveis();
		input(file);
		sortProcList();
		stagger();

	}

	private void initVariaveis() {
		this.time = 0;
		this.readProc = new ArrayList<PCB>();
		this.useDisk = false;

		this.newQueue = new ArrayList<PCB>();
		this.readyQueue = new ArrayList<PCB>();
		this.run = new ArrayList<PCB>();
		this.blockedQueue = new ArrayList<PCB>();
		this.exitQueue = new ArrayList<PCB>();
		this.currentStates = new ArrayList<PCB>();
	}

	private void input(String file) {
		String line;
		int start_time = 0;
		int pid = 0;

		//Tratamento de excecoes

		try {
			BufferedReader f = new BufferedReader(new FileReader(file));
			while ((line = f.readLine()) != null) {
				String[] array_line = line.split(" ");
				start_time = Integer.parseInt(array_line[0]);
				int[] auxiliar_PCB = new int[array_line.length-1];

				for (int i = 1; i < array_line.length; i++) {
					auxiliar_PCB[i-1] = Integer.parseInt(array_line[i]);
				}

				readProc.add(new PCB(pid, "Undefined", start_time, auxiliar_PCB));
				pid++;
			}
			f.close();
		}

		catch (IOException e) {
			e.printStackTrace();
		}
	}

	//Ordenar a lista de processos
	private void sortProcList() {
		Collections.sort(readProc, new Comparator<PCB>() {
			public int compare(PCB pcb1, PCB pcb2) {
				return pcb1.getTempo_inicio() - pcb2.getTempo_inicio();
			}
		});
	}

	private boolean stillProc() {
		return !this.readProc.isEmpty();
	}

	private int beginProc() {
		return this.readProc.get(0).getTempo_inicio();
	}

	private void createProc() {
		this.readProc.get(0).setStates("New");
		this.newQueue.add(this.readProc.remove(0));
	}

	private boolean blockedProc() {
		return !this.blockedQueue.isEmpty();
	}

	//Passar do estado "block" para o estado "ready"
	private void blockToReady() {
		this.blockedQueue.get(0).setStates("Ready");
		this.currentStates.set(this.currentStates.lastIndexOf(this.blockedQueue.get(0)), this.blockedQueue.get(0));
		this.readyQueue.add(this.blockedQueue.remove(0));
	}

	//Fazer o inverso, ou seja, passar do estado "ready" para o "block"
	private void readyToBlock() {
		this.readyQueue.get(0).setStates("Blocked");                     
		this.currentStates.set(this.currentStates.lastIndexOf(this.readyQueue.get(0)), this.readyQueue.get(0));
		this.filaBlocked.add(this.readyQueue.remove(0));
	}

	//Nova fila de processos
	private boolean newQueueProc() {
		return !this.newQueue.isEmpty();
	}

	//Pasagem do estado "new" para "ready"
	private void newToReady() {
		this.newQueue.get(0).setStatessetStates("Ready");
		this.currentStates.set(this.currentStates.lastIndexOf(this.newQueue.get(0)), this.newQueue.get(0));
		this.readyQueue.add(this.newQueue.remove(0));	
	}

	//Processo pronto
	private boolean readyProc() {
		return !this.readyQueue.isEmpty();
	}

	//Nao tem processos activos
	private boolean noProc() {
		return this.run.size() < 1;
	
	}

	//Corre o processo
	private void runProc() {
		this.readyQueue.get(0).setStates("Run");
		this.currentStates.set(this.currentStates.lastIndexOf(this.readyQueue.get(0)), this.readyQueue.get(0));
		this.run.add(this.readyQueue.remove(0));
	}

	private boolean runningProc() {
		return this.run.size() > 0;
	}

	private int instruction(ArrayList<PCB> queue) {
		return queue.get(0).getInstrucion()[queue.get(0).getProgramCounter()];
	}

	//O Programm Counter e incrementado
	private void pcIncrease(ArrayList<PCB> queue){ 												
		queue.get(0).setProgramCounter(queue.get(0).getProgramCounter() + 1);
	}

	//Passa do estado "run" para o "exit"
	private void runToExit(){ 									
		this.run.get(0).setStates("Exit");
		this.currentStates.set(this.currentStates.lastIndexOf(this.run.get(0)), this.run.get(0));
		this.exitQueue.add(this.run.remove(0));
		this.totalProc--;
	}

	//Vai passar do estado "run" para "ready"
	private void runToReady() {
		this.run.get(0).setStates("Ready");
		this.currentStates.set(this.currentStates.lastIndexOf(this.run.get(0)), this.run.get(0));
		this.readyQueue.add(this.run.remove(0));  
	}

	private boolean lastInstruction() {
		return this.run.get(0).getInstrucion().length == this.run.get(0).getProgramCounter()+1;
	}

	private void nullPC() {
		this.run.get(0).setProgramCounter(0);
	}

	//
	private void output() {
		System.out.print(time);
		for (int i = 0; i < currentStates.size(); i++)
			System.out.print(" | "+currentStates.get(i).getId()+" - "+currentStates.get(i).getStates());
		System.out.println();
	}

	

	private void stagger() {
		
		//Copia os processos para a lista currentStates para saber os estados dos processos
		//Podemos ver todoas as opcoes em termos de processos
		for (int i = 0; i < readProc.size(); i++)
			currentStates.add(readProc.get(i));

		while (this.time != this.timeOut) {

			if (blockedProc())  
				blockToReady();

			if (runningProc()){
				if (lastInstruction()){	
					runToExit();
					this.useDisk = false;
				}
				else {
					runToReady();  
					this.useDisk = false;
				}									
			}

			if (readyProc()) { 
				if (instruction(readyQueue) == 3) {
					pcIncrease(readyQueue);
					readyQueue.add(readyQueue.remove(0));
				}
				
				else if (noProc())
					runProc();
			}

			if (runningProc()) {  
				if (instruction(run) == 1) {
					pcIncrease(run);
					this.useDisk = false;
				}

				if (readyProc() && instruction(run) == 2 && instruction(readyQueue) == 2) {
					this.useDisk = true;
					pcIncrease(run);
					readyToBlock();
				}

				if (instruction(run) == 4) {
					nullPC();
					this.useDisk = false;
				}
				
				//FORK
				if (instruction(run) == 5) {
					PCB fork = this.run.get(0);
					fork.setId(this.currentStates.size());
					fork.setProgramCounter(0);
					
					if (this.totalProc <= 10)
						this.readyQueue.add(fork);
					else
						this.readProc.add(this.run.get(0));
					
					this.currentStates.add(fork);	
					pcIncrease(run);
					this.useDisk = false;
				}
			}

			if (newQueueProc())
				newToReady();

			if (stillProc() && beginProc() <= time && this.totalProc <= 10)
				createProc();
				

			output();
			time++;
		}
	}

	public static void main(String args[]) {
		new Escalonamento("input.txt", 200);
	}
}