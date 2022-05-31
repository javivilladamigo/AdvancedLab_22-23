# import
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from itertools import product
plt.style.use("classic")
import seaborn as sns

# file
def read_file(user):
    if user=='nicola':
        dirname = '/Users/nicola/Desktop/Advanced Laboratory A/data/20220517/252Cf_CLYC_both_COINCIDENCE_CH4_1/FILTERED/'
    if user=='vittoria':
        dirname = '/mnt/c/Users/UTENTE/Downloads/252Cf_CLYC_both_COINCIDENCE_CH4_1/FILTERED/'
    if user == "javi":
        dirname = "/Volumes/javimarino/Padova/AdvancedLab/data/20220517/60Co_252Cf_CLYC_both_COINCIDENCE/UNFILTERED/"
    filename_ch0 = dirname + 'CH_0@DT5725_1701_Data_60Co_252Cf_CLYC_both_1.csv'
    filename_ch2 = dirname + 'CH_2@DT5725_1701_Data_60Co_252Cf_CLYC_both_1.csv'
    filename_ch4 = dirname + 'CH_4@DT5725_1701_Data_60Co_252Cf_CLYC_both_1.csv'
    return filename_ch0, filename_ch2, filename_ch4

filename_ch0, filename_ch2, filename_ch4 = read_file('javi')

# retain data
df0 = pd.read_csv(filename_ch0, skiprows=1, header=None, sep=';')
df0.columns = (['TIMETAG', 'ENERGY', 'FLAGS'] + [x for x in range(len(df0.columns)-3)])
df2 = pd.read_csv(filename_ch2, skiprows=1, header=None, sep=';')
df2.columns = (['TIMETAG', 'ENERGY', 'FLAGS'] + [x for x in range(len(df2.columns)-3)])
df4 = pd.read_csv(filename_ch4, skiprows=1, header=None, sep=';')
df4.columns = (['TIMETAG', 'ENERGY', 'FLAGS'] + [x for x in range(len(df4.columns)-3)])


'''
   The getCoinc function finds the coincidences between 2 DataFrames 
   basing on the timetags. The function returns the two modified 
   DataFrames (removed the unmatched events and sorted in correct order
   the others). Inputs: DataFrame1, DataFrame2, tolerance in the 
   timetags matching. (mind the two DataFrames order!)
'''

def getCoinc(df1, df2, tol=10**5): #tolerance (ps)
    
    # sort dataframes
    df1 = df1.sort_values('TIMETAG')
    df2 = df2.sort_values('TIMETAG')

    # retrieve timestamps (numpy arrays)
    timetag1 = np.array(df1.loc[:, 'TIMETAG'])
    timetag2 = np.array(df2.loc[:, 'TIMETAG'])

    # modifying the temporary timetag2 array in the for loop 
    timetag2_temp = timetag2.copy() 

    couples, eliminate_row, diff = [], [], []
    counter = 0

    for n, i in enumerate(timetag1):
        coinc = np.argwhere(abs(timetag2_temp-i) <= tol) # find the index of timetag2 where the diff is smaller than tol
    
        if np.size(coinc) == 0: 
            
            eliminate_row.append(n)
            counter+=1 # times when this happens, necessary to correct the position to append in other cases
    
        elif np.size(coinc) == 1: # just one couple found
            
            couples.append([n, coinc[0][0] + (n-counter) ]) # append the timetag1 and timetag2 positions for the coincidence found
            timetag2_temp = np.delete(timetag2_temp, coinc[0][0]) # remove the values that have been associated
    
        else:
            for [position] in coinc: 
                diff.append(abs(timetag2_temp[position]-i)) # difference between the candidate timetag2 values and i
            min_index = diff.index(min(diff)) # find the minimum difference index in the diff list
            couples.append([n, coinc[min_index][0] + (n-counter)]) # append the best couple
            diff = [] 
            timetag2_temp = np.delete(timetag2_temp, coinc[min_index][0])
    
    # rearrange the second DataFrame row order to match the first one and remove the non associated rows of both DataFrames
    index_timetag2 = []
    for [i,j] in couples:
        index_timetag2.append(j)
        
    index_timetag1 = [x for x in range(len(df1.index)) if x not in eliminate_row]
    
    # create final DataFrames
    df1 = df1.loc[index_timetag1]
    df2 = df2.loc[index_timetag2]
    
    df1 = df1.set_index([pd.Index([i for i in range(len(df1.index)) ])])
    
    return df1, df2


df0_new, df2_new = getCoinc(df0, df2)




# function to get PSA from a dataframe
def getPSA(df, Nevent = 10000, nbaseline = 20, thr = 50, shortgate = 60, longgate = 250):

    '''
    parameters: df with the waveforms, Nevent is the total number of events to analyze,
    nbaseline is the number of samples where the baseline is calculated, thr is the 
    threshold value for starting the integration, qlong and qshort are the gates for the
    integration. The last two should not be modified
    '''

    psa = []

    #loop over the events
    for i in range(Nevent):
        # retrieve data
        waveform = np.array(df.iloc[i, 3:])

        # calculate baseline of the waveforms
        baseline = np.mean(waveform[3:nbaseline])

        # subtract baseline to waveforms
        waveform = waveform - baseline

        # calculation of starting point for integration
        start = 0
        for j in range(500):
            if waveform[j]>=thr and waveform[j+1]>=thr and waveform[j+2]>=thr and waveform[j+3]>=thr:
                aux = j
                start = aux
                break
            
        # calculation of qlong and qshort (from bin 50 on) for channel 2
        qlong, qshort = 0., 0.
        for j in np.arange(start, start+longgate):
            if j < start+shortgate : qshort += waveform[j]
            qlong += waveform[j]

        # calculation of PSA for channel 2
        aux_psa = (qlong - qshort) / qlong
        psa.append(aux_psa)

    return psa


nevents = 10000

# retrieving the psa for the two detectors
psa_MI = getPSA(df2, nevents)
psa_PD = getPSA(df4, nevents)




# defining final dataframe for both CLYCs
df_PD = pd.DataFrame({ 'TIMETAG' : df4.iloc[:nevents, 0], 'Energy' : df4.iloc[:nevents, 1], 'PSA' : psa_PD })
df_MI = pd.DataFrame({ 'TIMETAG' : df0_new.iloc[:nevents, 0], 'Energy' : df0_new.iloc[:nevents, 1], 'PSA' : psa_MI })




df_PD_new, df_MI_new = getCoinc(df_PD, df_MI)



df_final = pd.concat([df_PD_new, df_MI_new], axis=1, join='inner')
df_final = df_final.drop('TIMETAG', 1)
df_final.columns = ['ENERGY_PD', 'PSA_PD', 'ENERGY_MI', 'PSA_MI']





# define energy thresholds and PSA cuts

# energy threshold
threshold_PD = 200
threshold_MI = 800

# gamma cuts
PSArange_gamma_PD = [0.55, 0.60]
PSArange_gamma_MI = [0.53, 0.58]

# neutron cuts
PSArange_neutron_PD = [0.60, 1]
PSArange_neutron_MI = [0.58, 1]







subth_mask = df_final[(df_final["ENERGY_PD"] < threshold_PD) | (df_final["ENERGY_MI"] < threshold_MI)].index
df_final.drop(subth_mask, inplace = True)


gamma_mask = df_final["PSA_PD"] < PSArange_gamma_PD[1]; neutron_mask = df_final["PSA_PD"] > PSArange_neutron_PD[0]
df_final.loc[gamma_mask, "pID_PD"] = "gamma"
df_final.loc[neutron_mask, "pID_PD"] = "neutron"

gamma_mask = df_final["PSA_MI"] < PSArange_gamma_PD[1]; neutron_mask = df_final["PSA_MI"] > PSArange_neutron_PD[0]
df_final.loc[gamma_mask, "pID_MI"] = "gamma"; df_final.loc[neutron_mask, "pID_MI"] = "neutron"









count_coincidences = df_final.groupby(['pID_PD', 'pID_MI']).size()





fig, ax = plt.subplots(1, 2, figsize=(15, 6)); fig.patch.set_facecolor('xkcd:white')
for i in range(len(ax)):
    ax[i].tick_params(which = 'both', axis = 'both', direction='in', right = True, top = True, length = 6)
    ax[i].minorticks_on()
    ax[i].tick_params(which = 'minor', axis = 'both', direction='in', right = True, top = True, length = 3)
    ax[i].tick_params(axis = 'both', labelsize = 12.5)
h2_PD = ax[0].hist2d(df_PD['Energy'], df_PD['PSA'], bins = (1000,1000), cmap = "cool") # afmhot, hot, gist_heat are others that work reasonably well
fig.colorbar(h2_PD[3], ax=ax[0])

h2_MI = ax[1].hist2d(df_MI['Energy'], df_MI['PSA'], bins = (1000,1000), cmap = "cool")
fig.colorbar(h2_MI[3], ax=ax[1])

for i in range(len(ax[:])):
    ax[i].set_ylim(0.5, 0.9)


ax[0].set_xlim(50., 500)
ax[1].set_xlim(100, 3000)

fig.show()








# correlated gamma energies
gg_energy = np.array(df_final["ENERGY_PD"][df_final[ (df_final["pID_PD"] == "gamma") & (df_final["pID_MI"] == "gamma") ].index]), np.array(df_final["ENERGY_MI"][df_final[ (df_final["pID_PD"] == "gamma") & (df_final["pID_MI"] == "gamma") ].index])

neutrons_energy = np.hstack((np.array(df_final["ENERGY_PD"][df_final[ (df_final["pID_PD"] == "neutron") & (df_final["pID_MI"] == "gamma") ].index]), np.array(df_final["ENERGY_MI"][df_final[ (df_final["pID_MI"] == "neutron") & (df_final["pID_PD"] == "gamma") ].index])))
gammas_energy = np.hstack((np.array(df_final["ENERGY_PD"][df_final[ (df_final["pID_PD"] == "gamma") & (df_final["pID_MI"] == "neutron") ].index]), np.array(df_final["ENERGY_MI"][df_final[ (df_final["pID_MI"] == "gamma") & (df_final["pID_PD"] == "neutron") ].index])))
ng_energy = [neutrons_energy, gammas_energy]

nn_energy = np.array(df_final["ENERGY_PD"][df_final[ (df_final["pID_PD"] == "neutron") & (df_final["pID_MI"] == "neutron") ].index]), np.array(df_final["ENERGY_MI"][df_final[ (df_final["pID_PD"] == "neutron") & (df_final["pID_MI"] == "neutron") ].index])





sns.set_theme(context = "notebook", style = "white", palette = "bright")
sns.set_palette("flare")

g = sns.jointplot(x = gg_energy[0], y = gg_energy[1], kind = "reg", scatter = False)
g.ax_joint.scatter(gg_energy[0], gg_energy[1], marker = "o", alpha = 0.15, edgecolors = "black")
g.ax_joint.set_xlabel("gamma energy (PD) a.u.", fontweight='bold')
g.ax_joint.set_ylabel("gamma energy (MI) a.u.", fontweight='bold')
g.fig.suptitle("gamma-gamma correlation", fontweight = "bold", fontsize = 14.5)

g = sns.jointplot(x = ng_energy[0], y = ng_energy[1], kind = "reg", scatter = False)
g.ax_joint.scatter(ng_energy[0], ng_energy[1], marker = "o", alpha = 0.15, edgecolors = "black")
g.ax_joint.set_xlabel("neutron energy a.u.", fontweight='bold')
g.ax_joint.set_ylabel("gamma energy a.u.", fontweight='bold')
g.fig.suptitle("neutron-gamma correlation", fontweight = "bold", fontsize = 14.5)


g = sns.jointplot(x = nn_energy[0], y = nn_energy[1], kind = "reg", scatter = False)
g.ax_joint.scatter(nn_energy[0], nn_energy[1], marker = "o", alpha = 0.15, edgecolors = "black")
g.ax_joint.set_xlabel("neutron energy a.u.", fontweight='bold')
g.ax_joint.set_ylabel("neutron energy a.u.", fontweight='bold')
g.fig.suptitle("neutron-neutron correlation", fontweight = "bold", fontsize = 14.5)