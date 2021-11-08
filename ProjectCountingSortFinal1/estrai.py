#
# Course: High Performance Computing 2021/2022
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Avella Antonello            a.avella19@studenti.unisa.it             
# D'Andrea Anna               a.dandrea26@studenti.unisa.it 
# De Pisapia Claudio          c.depisapia1@studenti.unisa.it
# 
# Purpose: 
# this extract.py file contains the instructions in python for displaying 
# the speedup graphs and generating data in CSV format about the times
#
# Copyright (C) 2021 - All Rights Reserved
#
# This file is part of ContestOMP.
#
# ContestOMP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ContestOMP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ContestOMP.  If not, see <http://www.gnu.org/licenses/>.
#

import os
import re
import pandas as pd
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
import seaborn as sns
from prettytable import PrettyTable
from prettytable import MARKDOWN
from prettytable import MSWORD_FRIENDLY

config = {

			'timeInt': {

				'jpg': True,
				'speedup': True

			},
			'user': {

				'jpg': False,
				'speedup': False

				},
			'sys': {

				'jpg': False,
				'speedup': False

				},
			'real': {

				'jpg': False,
				'speedup': False

				}
		}


def _extract(folderPath, plotColumns):
    prev = os.getcwd()
    os.chdir(folderPath)

    # List diresctory
    # filenames =  [f for f in os.listdir('.') if os.path.isfile(f)]
    if not os.path.exists("jpg"):
        os.mkdir("jpg")

    # Remove not csv files
    filenames = [f for f in os.listdir('.') if f.endswith(".csv") and re.match("SIZE-[0-9]+-NTH-[0-9]{2}-O[0-9]-?[0-9]*", f)]
    print(filenames)

    filenames = sorted(filenames)
    means = {}

    for filename in filenames:
        file_mean = {}
        print('Processing : ' + filename)
        ds = pd.read_csv(filename, sep=';')

        for col in plotColumns.keys():
            print('Processing : ' + filename + ", Col : " + col)
            # extract the selected column
            x_data = ds[col]
            if(x_data.min()!=x_data.max()):
                mean,std=stats.norm.fit(x_data)
                # 68,3% = P{ μ − 1,00 σ < X < μ + 1,00 σ }
                x_data = ds[(ds[col] < (mean + std)) & (ds[col] > (mean - std))][col]
                mean,std=stats.norm.fit(x_data)
                file_mean[col] = mean
            
                if plotColumns[col]['jpg']:
                    sns.histplot(x_data, kde=True)
                    plt.savefig("jpg/" + str(col)+ "_" + filename.split('.')[0] + ".jpg")
                    plt.close()
            else:
                mean = x_data.min()
                file_mean[col] = mean
                
        means[filename] = file_mean
    os.chdir(prev)
    return means


def _compute_speedup(t,tp,nt):
	speedup = t/tp
	efficiency = t/(tp*float(nt))
	return speedup,efficiency

def _make_table(header,rows,print_table=False,save=True,name=""):
	if save and not name:
		raise Exception("No filename to save file")
	x = PrettyTable()
	x.field_names = header
	x.add_rows(rows)
	if save:
		_save_table(x,name)
	if print_table:
		print(x)
	return x

def _save_table(table,filename):
	with open(filename,"w") as table_file:
		#table.set_style(MARKDOWN)
		table.set_style(MSWORD_FRIENDLY)
		data = table.get_string()
		table_file.write(data)

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


if __name__ == "__main__":
    root=os.path.join(os.path.dirname(os.path.realpath(__file__)),"measure/")
    folders = [f for f in os.listdir(root) if (os.path.isdir(os.path.join(root,f) ))]
    # print(f"Found folders : {folders}")

    choosedFolder  = os.path.join(root,max(folders))

    for folder in os.listdir(choosedFolder):
        #print(f"Folder : {folder}")
        actualPath = os.path.join(choosedFolder,folder)
        means = _extract(actualPath,config)

        header = {'values':['Version','Threads','TimeInt','User','Sys','Real','Speedup','Efficiency']}
        cells = {'values':[]}
        nt = -1
        for filename_key in means:
            cell = []
            splitted_filename=filename_key.split("-")
            if "NTH-00" in filename_key:
                for col in config:
                    if config[col]['speedup']:
                        seq = means[filename_key][col]
                nt = 1
                cell.append('Serial')
                cell.append(nt)
            else:
                nt = int(splitted_filename[3])
                cell.append('Parallel')
                cell.append(nt)
            
            for col in config:
                cell.append(means[filename_key][col])
                if config[col]['speedup']:
                    speedup,efficiency = _compute_speedup(seq,means[filename_key][col],nt)
            cell.append(speedup)
            cell.append(efficiency)
            
            cells['values'].append(cell)
            
        splitted_folder = folder.split("-")
        size = splitted_folder[1]
        opt = splitted_folder[4]
        table_filename = actualPath + "/psize-" + size + "-" + str(opt) + "-table.csv"
        plot_filename = actualPath + "/speedup-" + str(size) + "-" + str(opt) +  ".jpg"

        table = _make_table(header['values'],cells['values'],name=table_filename)
        _plot_from_table(header["values"],cells["values"],name=plot_filename)


