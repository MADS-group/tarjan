#  
# Course: High Performance Computing 2022/2023 

# Lecturer: Francesco Moscato  fmoscato@unisa.it 

# Group:
#  Langella	Antonio   0622702011   a.langella31@studenti.unisa.it
#  Marsico Michele   0622702012   m.marsico10@studenti.unisa.it
#  Paolino Salvatore   0622702016   s.paolino6@studenti.unisa.it 
#  Risi Davide   0622702013   d.risi2@studenti.unisa.it
 
#  Copyright (C) 2023 - All Rights Reserved 

#  This file is part of Parallelize-Tarjan-MPI-CUDA. 

#  Parallelize-Tarjan-MPI-CUDA is free software: you can redistribute it and/or modify 
#  it under the terms of the GNU General Public License as published by 
#  the Free Software Foundation, either version 3 of the License, or 
#  (at your option) any later version. 

#  Parallelize-Tarjan-MPI-CUDA is distributed in the hope that it will be useful, 
#  but WITHOUT ANY WARRANTY; without even the implied warranty of 
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
#  GNU General Public License for more details. 

#  You should have received a copy of the GNU General Public License 
#  along with Parallelize-Tarjan-MPI-CUDA.  If not, see <http://www.gnu.org/licenses/>. 
#  

import os
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import pandas as pd
from scipy import stats
import seaborn as sns
from prettytable import PrettyTable
from prettytable import MARKDOWN
from prettytable import MSWORD_FRIENDLY
import re

execution_type = "mpi"

config_sequential = {
				'vertices':{
					'jpg':False,
					'speedup':False
				},
				'init':{
					'jpg':False,
					'speedup':False
				},
				'destroy':{
					'jpg':False,
					'speedup':False
				},
				'tarjan':{
					'jpg':False,
					'speedup':False
				},
				'user':{
					'jpg':False,
					'speedup':False
					},
				'system':{
					'jpg':False,
					'speedup':False
					},
				'pCPU':{
					'jpg':False,
					'speedup':False
					},
				'elapsed':{
					'jpg':True,
					'speedup':True
					}
				}

config_sequential_pre = {		
				'verteces':{
					'jpg':False,
					'speedup':False
				},
				'verteces_after':{
					'jpg':False,
					'speedup':False
				},
				'init':{
					'jpg':False,
					'speedup':False
				},
				'destroy':{
					'jpg':False,
					'speedup':False
				},
				'tarjan':{
					'jpg':False,
					'speedup':False
				},
				'preprocess_time':{
					'jpg':False,
					'speedup':False
				},
				'user':{
					'jpg':False,
					'speedup':False
					},
				'system':{
					'jpg':False,
					'speedup':False
					},
				'pCPU':{
					'jpg':False,
					'speedup':False
					},
				'elapsed':{
					'jpg':True,
					'speedup':True
					}
				}


config = {}
if execution_type == "sequential":

	config.update( { 
				'vertices':{
					'jpg':False,
					'speedup':False
				},
				'init':{
					'jpg':False,
					'speedup':False
				},
				'destroy':{
					'jpg':False,
					'speedup':False
				},
				'tarjan':{
					'jpg':False,
					'speedup':False
				}
			})
elif execution_type == "cuda_mpi":
	config.update( {	
				'Nvert':{
					'jpg':False,
					'speedup':False
				},
				'NvertAfterCuda':{
					'jpg':False,
					'speedup':False
				},
				'init':{
					'jpg':False,
					'speedup':False
				},
				'mpi_tarjan':{
					'jpg':False,
					'speedup':False
				},
				'split':{
					'jpg':False,
					'speedup':False
				},
				'merge':{
					'jpg':False,
					'speedup':False
				},
				'total_only_mpi':{
					'jpg':False,
					'speedup':False
				},
				'preprocess':{
					'jpg':False,
					'speedup':False
				},
				'conversion':{
					'jpg':False,
					'speedup':False
				},
				'finalize':{
					'jpg':False,
					'speedup':False
				}
			})
elif execution_type == "cuda":
	config.update( {	
				'verteces':{
					'jpg':False,
					'speedup':False
				},
				'init':{
					'jpg':False,
					'speedup':False
				},
				'finalize':{
					'jpg':False,
					'speedup':False
				},
				'preprocess':{
					'jpg':False,
					'speedup':False
				},
				'conversion':{
					'jpg':False,
					'speedup':False
				},
				'tarjan':{
					'jpg':False,
					'speedup':False
				}
			})
elif execution_type == "mpi":
	config.update( {	
				'Nvert':{
					'jpg':False,
					'speedup':False
				},
				'init':{
					'jpg':False,
					'speedup':False
				},
				'tarjan':{
					'jpg':False,
					'speedup':False
				},
				'split':{
					'jpg':False,
					'speedup':False
				},
				'merge':{
					'jpg':False,
					'speedup':False
				}
			})
elif execution_type == "sequential_pre":
		config.update( {	
				'verteces':{
					'jpg':False,
					'speedup':False
				},
				'vertices_after':{
					'jpg':False,
					'speedup':False
				},
				'init':{
					'jpg':False,
					'speedup':False
				},
				'destroy':{
					'jpg':False,
					'speedup':False
				},
				'tarjan':{
					'jpg':False,
					'speedup':False
				},
				'preprocess_time':{
					'jpg':False,
					'speedup':False
				}
			})

config.update({
				'user':{
					'jpg':False,
					'speedup':False
					},
				'system':{
					'jpg':False,
					'speedup':False
					},
				'pCPU':{
					'jpg':False,
					'speedup':False
					},
				'elapsed':{
					'jpg':True,
					'speedup':True
					}
			})

def _extract(path_to_folder,plot_columns):
	prev = os.getcwd()
	os.chdir(path_to_folder)

	#List diresctory
	filenames =  [f for f in os.listdir('.') if os.path.isfile(f)]
	if not os.path.exists("jpg"):
		os.mkdir("jpg")

	#Remove not csv files
	filenames = [f for f in os.listdir('.') if f.endswith(".csv") and ("graph_type" in f) ]
	
	filenames = sorted(filenames)
	means = {}
	
	for filename in filenames:
		file_mean = {}
		print('Processing : ' + filename)
		ds = pd.read_csv(filename)#,lineterminator = ";")
		def f(x): 
			x = x[:-1]
			return int(x) 
		ds['pCPU'] = ds['pCPU'].apply(f)
		for col in plot_columns.keys():
			print('Processing : ' + filename + ", Col : " + col)
			#extract the selected column
			x_data = ds[col]
			mean,std=stats.norm.fit(x_data)
			#68,3% = P{ μ − 1,00 σ < X < μ + 1,00 σ }
			#TODO: Capire se lasciarlo (da problemi)
			#x_data = ds[(ds[col] < (mean + std)) & (ds[col] > (mean - std))][col]
			mean,std=stats.norm.fit(x_data)
			file_mean[col] = mean
			
			if plot_columns[col]['jpg']:
				sns.histplot(x_data, kde=True)
				plt.savefig("jpg/" + str(col)+ "_" + filename.split('.')[0] + ".jpg")
				plt.close()
			
		means[filename] = file_mean
	os.chdir(prev)
	return means

def _compute_speedup(t,tp,nt,psize):
	speedup = t/tp
	efficiency = t/(tp*float(nt))
	return speedup,efficiency

def _make_table(header,rows,print_table=False,save=True,name=""):
	if save and not name:
		raise Exception("No filename to save file")
	x = PrettyTable()
	x.field_names = header
	print(header)
	print(rows)
	df =  pd.DataFrame(data=rows, columns=header)
	x.add_rows(rows)
	if save:
		#_save_table(x,name)
		_save_table_to_latex(df,name)
	if print_table:
		print(x)
	return x

def _save_table(table,filename):
	with open(filename,"w") as table_file:
		#table.set_style(MARKDOWN)
		table.set_style(MSWORD_FRIENDLY)
		data = table.get_string()
		table_file.write(data)

def _save_table_to_latex(df: pd.DataFrame,filename):
	with open(filename,"w") as table_file:
		column_format = ""
		for col in df.columns:
			column_format += "|c"
		column_format += "|"
		table_file.write(df.to_latex(index=False, float_format="{:0.3f}".format, column_format=column_format))

def _plot_from_table(header,rows,save=True,name="",show_plot=False):
	if save and not name:
		raise Exception("No filename to save file")

	x = [0]
	y = [0]
	speedup_pos = header.index("Speedup")
	thread_pos = header.index("Threads")
	for row in rows[1:]:
		x.append(row[thread_pos])
		y.append(row[speedup_pos])

	x_th = np.array(x)
	fig, ax = plt.subplots(figsize=(12, 8))
	ax.plot(x_th, y, 'ro-', label='Experimental')
	ax.plot(x_th, x_th, color='blue', label='Ideal')
	#same as y_th, bisection
	plt.style.use('seaborn-whitegrid')

	plt.autoscale(enable=True, axis='x', tight=True)
	plt.autoscale(enable=True, axis='y', tight=True)	

	plt.legend()
	plt.xlabel("Processors")
	plt.ylabel("Speedup")
	if show_plot:
		plt.show()
	if save:
		plt.savefig(name)
	plt.close()

def extraction(root=os.path.join(os.path.dirname(os.path.realpath(__file__)),"measure/" + execution_type), cols=config, threads=[0,1,2,4,8]):
	print("Listing folder for problem size")
	folders = [f for f in os.listdir(root) if (os.path.isdir(os.path.join(root,f)) and re.match("graph_type*",f))]
	print(f"Found folders : {folders}")
	#print(cols)
	for folder in folders:
		print(f"Folder : {folder}")
		joined_path = os.path.join(root,folder)
		if execution_type == "mpi":
			means = _extract(os.path.join(os.path.dirname(os.path.realpath(__file__)),"measure/sequential",folder),config_sequential)
		elif execution_type == "cuda" or execution_type == "cuda_mpi":
			means = _extract(os.path.join(os.path.dirname(os.path.realpath(__file__)),"measure/sequential_pre",folder),config_sequential_pre)
		
		temp = _extract(joined_path,cols)
		means.update(temp)
		
		#print(means)
		if execution_type == "mpi":
		    header = {'values':['Version','Threads','Nvert','init','tarjan','split','merge','user','system','pCPU','elapsed','Speedup','Efficiency']}
		elif execution_type ==  "sequential_pre":
			header = {'values':['Version','Threads','verteces','verteces_after','init','destroy','tarjan','preprocess_time','user','system','pCPU','elapsed','Speedup','Efficiency']}
		elif execution_type == "sequential":
		    #header = {'values':['vertices','init','destroy','tarjan','user','system','elapsed','pCPU']}
			header = {'values':['Version','Threads','vertices','init','destroy','tarjan','user','system','pCPU','elapsed','Speedup','Efficiency']}
		elif execution_type == "cuda_mpi":
		    header = {'values':['Version','Threads','Nvert','NvertAfterCuda','init','mpi_tarjan','split','merge','total_only_mpi','preprocess','conversion','finalize','user','system','pCPU','elapsed','Speedup','Efficiency']}
		elif execution_type == "cuda":
		    header = {'values':['Version','Threads','verteces','init','finalize','preprocess','conversion','tarjan','user','system','pCPU','elapsed','Speedup','Efficiency']}
		else:
		    print("Error!")
        
		#header = {'values':['Version','Threads','Init','Dotprod','User','Sys','Elapsed','Speedup','Efficiency']}
		
		cells = {'values':[]}
		nt = -1
		for filename_key in means:
			cell = []
			splitted_filename=filename_key.split("-")
			if "sequential" in filename_key:
				seq = means[filename_key]['elapsed']
				nt = 1
				cell.append('Serial')
				cell.append(nt)
			else:
				#print(splitted_filename[-1].split(".")[0])
				nt = int(splitted_filename[-1].split(".")[0])
				cell.append('Parallel')
				cell.append(nt)

			for col in cols:
				if col == 'Version' or col == 'Threads':
					continue
				if col in means[filename_key]:
					cell.append(means[filename_key][col])
					if cols[col]['speedup']:
						psize = splitted_filename[1]
						speedup,efficiency = _compute_speedup(seq,means[filename_key][col],nt,psize)
						cell.append(speedup)
						cell.append(efficiency)
				else:
					cell.append("no data")
				
			cells['values'].append(cell)
		
		
		splitted_folder = folder.split("-")
		size = splitted_folder[1]
		opt = splitted_folder[2]
		table_filename = joined_path + "/psize-" + size + "-" + str(opt) + "-table.tex"
		plot_filename = joined_path + "/speedup-" + str(size) + "-" + str(opt) +  ".jpg"

		table = _make_table(header['values'],cells['values'],name=table_filename)
		_plot_from_table(header["values"],cells["values"],name=plot_filename)

if __name__ == "__main__":
	extraction()
