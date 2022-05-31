# import
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import zipfile
import os

# file
def read_file(user):
    if user=='nicola':
        dirname = '/Users/nicola/Desktop/Advanced Laboratory A/data/20220524/'
    if user=='vittoria':
        dirname = '/mnt/c/Users/UTENTE/Downloads/'
    if user == "javi":
        dirname = "/Users/javi/Desktop/channels_concat/"
    filename_ch_MI = dirname + 'ch0_20220524.pkl'
    filename_ch_PD = dirname + 'ch4_20220524.pkl'
    return dirname, filename_ch_MI, filename_ch_PD

print('Reading files...\n')

dirname, filename_ch_MI, filename_ch_PD = read_file('javi')

'''# unzip file
with zipfile.ZipFile(filename_ch_MI + '.zip', 'r') as zip_ref:
    zip_ref.extractall(dirname)
with zipfile.ZipFile(filename_ch_PD + '.zip', 'r') as zip_ref:
    zip_ref.extractall(dirname)
'''

# retain data
print('Creating DataFrames...\n')
df_MI = pd.read_pickle(filename_ch_MI)
df_PD = pd.read_pickle(filename_ch_PD)

# reset index because pickle data is not well formatted
df_MI.reset_index(drop=True, inplace=True)
df_PD.reset_index(drop=True, inplace=True)
print('Dataset retrieved!')


'''# remove the unzipped file
os.remove(filename_ch_MI)
os.remove(filename_ch_PD)'''

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

    couples, diff = [], []
    index_timetag1 = []
    counter = 0

    ntot = len(timetag1)

    for n, i in enumerate(timetag1):

        print('Analyzing event %d, %.1f%%'%(n, n/ntot*100), end='\r')

        coinc = np.argwhere(abs(timetag2_temp-i) <= tol) # find the index of timetag2 where the diff is smaller than tol

        if np.size(coinc) == 0:

            counter+=1 # times when this happens, necessary to correct the position to append in other cases

        elif np.size(coinc) == 1: # just one couple found

            couples.append([n, coinc[0][0] + (n-counter) ]) # append the timetag1 and timetag2 positions for the coincidence found
            timetag2_temp = np.delete(timetag2_temp, coinc[0][0]) # remove the values that have been associated
            index_timetag1.append(n)

        else:
            for [position] in coinc:
                diff.append(abs(timetag2_temp[position]-i)) # difference between the candidate timetag2 values and i
            min_index = diff.index(min(diff)) # find the minimum difference index in the diff list
            couples.append([n, coinc[min_index][0] + (n-counter)]) # append the best couple
            diff = []
            timetag2_temp = np.delete(timetag2_temp, coinc[min_index][0])
            index_timetag1.append(n)


    print('Search for coincidence completed!\n')
    print('Rearranging second dataframe...\n', end=' ')
    # rearrange the second DataFrame row order to match the first one and remove the non associated rows of both DataFrames
    index_timetag2 = []
    for [i,j] in couples:
        index_timetag2.append(j)

    #print('Creating index for the timestamp...\n')
    #index_timetag1 = [x for x in range(len(df1.index)) if x not in eliminate_row]

    print('Creating final dataframe...\n', end=' ')
    # create final DataFrames
    df1 = df1.loc[index_timetag1]
    df2 = df2.loc[index_timetag2]

    print('Setting correct index...\n')
    df1 = df1.set_index([pd.Index([i for i in range(len(df1.index)) ])])

    print('Finished')

    return df1, df2

df_MI_new, df_PD_new = getCoinc(df_MI, df_PD)


print('Saving first dataframe...')
df_MI_new.to_pickle('dfMI_psa_coinc_20220524.pkl')
print('Saving second dataframe...')
df_PD_new.to_pickle('dfPD_psa_coinc_20220524.pkl')
