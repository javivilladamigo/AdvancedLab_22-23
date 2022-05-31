# import
import numpy as np
import pandas as pd
import zipfile
import os
from pandarallel import pandarallel

# file
def read_file(user):
    if user=='nicola':
        dirname = '/Users/nicola/Desktop/Advanced Laboratory A/data/20220524/'
    if user=='vittoria':
        dirname = '/mnt/c/Users/UTENTE/Downloads/'
    if user == "javi":
        dirname = "//Volumes/javimarino/Padova/AdvancedLab/data/20220524/"
    filename_ch0 = dirname + 'ch0_coincidence_20220524.pkl'
    filename_ch2 = dirname + 'ch2_coincidence_20220524.pkl'
    filename_ch4 = dirname + 'ch4_20220524.pkl'
    return dirname, filename_ch0, filename_ch2, filename_ch4

print('Reading files...')

dirname, filename_ch0, filename_ch2, filename_ch4 = read_file('javi')


# retain data
df0 = pd.read_pickle(filename_ch0)
df2 = pd.read_pickle(filename_ch2)
#df4 = pd.read_pickle(filename_ch4)

# reset index of dataframe
df0.reset_index(drop=True, inplace=True)
df2.reset_index(drop=True, inplace=True)
#df4.reset_index(drop=True, inplace=True)

# remove the unzipped file
#os.remove(filename_ch0)
#os.remove(filename_ch2)
#os.remove(filename_ch4)

print('Data retrieved!')

# function to get PSA from a dataframe
def getPSA_fromline(line, shortgate = 60, longgate = 250):

    nbaseline = 20
    thr = 50
    
    # retrieve data
    waveform = np.array(line[3:])

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
    psa = (qlong - qshort) / qlong

    return psa

# retrieving the psa for the two detectors
pandarallel.initialize()
print('Analyzing 1st dataframe...')
df2['PSA'] = df2.parallel_apply(getPSA_fromline, axis=1, args=(60, 250))
#print('Analyzing 2nd dataframe...')
#df4['PSA'] = df4.parallel_apply(getPSA_fromline, axis=1, args=(40, 250))

# defining final dataframe for both CLYCs
#df_PD = pd.DataFrame({ 'TIMETAG' : df4['TIMETAG'], 'Energy' : df4['ENERGY'], 'PSA' : df4['PSA'] })
df_MI = pd.DataFrame({ 'TIMETAG' : df2['TIMETAG'], 'Energy' : df0['ENERGY'], 'PSA' : df2['PSA'] })

path_to_save = "/Volumes/MARINO_HD/Padova/AdvancedLab/data/20220524/"
#print('Saving first dataframe...')
#df_PD.to_pickle(path_to_save+'dfPD_psa_20220524.pkl')
print('Saving second dataframe...')
df_MI.to_pickle(path_to_save+'dfMI_psa_test.pkl')

print('Finished!')