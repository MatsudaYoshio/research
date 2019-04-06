import matplotlib.colors as colors
import matplotlib.pyplot as plt
import numpy as np
import csv
import statistics
import math

questions_num = 7

dimensions = ["comfort", "fun", "low physical\nfatigue", "low mental\nfatigue", "ease of use", "efficiency", "low\ninterference"]

opt_data = [[] for i in range(questions_num)]
fix_data = [[] for i in range(questions_num)]

with open('experiment_data.csv', 'r') as f:
    reader = csv.reader(f)
    header = next(reader)  # ヘッダーを読み飛ばす

    for row in reader:
        for i in range(questions_num):
            opt_data[i].append(row[12+i])
        for i in range(questions_num):
            fix_data[i].append(row[12+questions_num+i])

opt_mean = []
opt_error = []
for q in opt_data:
    q_data = [int(x) for x in q]
    opt_mean.append(statistics.mean(q_data))
    opt_error.append(statistics.pstdev(q_data)/math.sqrt(len(q_data)))

fix_mean = []
fix_error = []
for q in fix_data:
    q_data = [int(x) for x in q]
    fix_mean.append(statistics.mean(q_data))
    fix_error.append(statistics.pstdev(q_data)/math.sqrt(len(q_data)))
 
left = np.arange(questions_num)  # numpyで横軸を設定
 
width = 0.3

# texでずれないようにする
plt.rcParams['ps.useafm'] = True
plt.rcParams['pdf.use14corefonts'] = True
plt.rcParams['text.usetex'] = True

fig = plt.figure(figsize=(22, 10), dpi=220)
plt.grid(which='major',axis='y',color='black',linestyle='-',alpha=0.25)
plt.bar(left, opt_mean, color=colors.cnames['hotpink'], width=width, yerr = opt_error, capsize = 6, label="optimized", align='center')
plt.bar(left+width, fix_mean, color=colors.cnames['deepskyblue'], width=width, yerr = fix_error, capsize = 6, label="fixed", align='center')
plt.ylim([1,5])
plt.yticks([1,2,3,4,5], fontsize=34)
plt.xticks(left + width/2, dimensions, fontsize=34)
plt.xlabel("questionnaire", fontsize=48, weight='bold')
plt.ylabel("mean ratings", fontsize=48, weight='bold')
plt.legend(loc="best", fontsize=34)
plt.savefig('questionnaire_result.eps', bbox_inches='tight', pad_inches=0.14)
plt.show()
