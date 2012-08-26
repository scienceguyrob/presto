#include "backend_common.h"
#include "sigproc_fb.h"

typedef enum {
    SIGPROCFB, PSRFITS, SCAMP, BPP, WAPP, GMRT, SPIGOT, 
    SUBBAND, DAT, SDAT, EVENTS, UNSET;
} psrdatatype;

#define RAWDATA (cmd->pkmbP || cmd->bcpmP || cmd->wappP || \
                 cmd->gmrtP || cmd->spigotP || cmd->filterbankP || \
                 cmd->psrfitsP)

void identify_psrdatatype(spectra_info *s, int output)
{
    char *root, *suffix;
    
    /* Split the filename into a rootname and a suffix */
    if (split_root_suffix(s->filenames[0], &root, &suffix) == 0) {
        printf("\nThe input filename (%s) must have a suffix!\n\n", s->filenames[0]);
        exit(1);
    } else {
        if (strcmp(suffix, "dat") == 0) {
            if (output) printf("Assuming the data are a PRESTO dat time series...\n");
            s->datatype = DAT;
        } else if (strcmp(suffix, "sdat") == 0) {
            if (output) printf("Assuming the data are a PRESTO sdat time series...\n");
            s->datatype = SDAT;
        } else if (strncmp(suffix, "sub", 3) == 0) {
            if (output) printf("Assuming the data are in PRESTO subband format...\n");
            s->datatype = SUBBAND;
        } else if (strcmp(suffix, "events") == 0) {
            if (output) printf("Assuming the data are an event list...\n");
            s->datatype = EVENTS;
        } else if (strcmp(suffix, "bcpm1") == 0 || strcmp(suffix, "bcpm2") == 0) {
            if (output) printf("Assuming the data are from a GBT BCPM...\n");
            s->datatype = BPP;
        } else if (strcmp(suffix, "fil") == 0 || strcmp(suffix, "fb") == 0) {
            if (output) printf("Assuming the data are in SIGPROC filterbank format...\n");
            s->datatype = SIGPROCFB;
        } else if (strcmp(suffix, "fits") == 0) {
            if (strstr(root, "spigot_5") != NULL) {
                if (output) printf("Assuming the data are from the NRAO/Caltech Spigot...\n");
                s->datatype = SPIGOT;
            } else if (is_PSRFITS(filename)) {
                if (output) printf("Assuming the data are in PSRFITS search-mode format...\n");
                s->datatype = PSRFITS;
            } 
        } else if (strcmp(suffix, "pkmb") == 0) {
            if (output) printf("Assuming the data are in 'SCAMP' 1-bit filterbank system...\n");
            s->datatype = SCAMP;
        } else if (strncmp(suffix, "gmrt", 4) == 0) {
            if (output) printf("Assuming the data are from the GMRT Phased Array system...\n");
            s->datatype = GMRT;
        } else if (isdigit(suffix[0]) && isdigit(suffix[1]) && isdigit(suffix[2])) {
            if (output) printf("Assuming the data are from the Arecibo WAPP system...\n");
            s->datatype = WAPP;
        }
    }
    free(root);
    free(suffix);
}



void psrdatatype_description(char *outstr, psrdatatype type)
    switch(psrdatatype) {
    case SIGPROCFB:
        strcpy(outstr, "SIGPROC filterbank");
        break;
    case PSRFITS:
        strcpy(outstr, "PSRFITS");
        break;
    case SCAMP:
        strcpy(outstr, "SCAMP 1-bit filterbank");
        break;
    case BPP:
        strcpy(outstr, "GBT BCPM");
        break;
    case GMRT:
        strcpy(outstr, "GMRT simple");
        break;
    case SPIGOT:
        strcpy(outstr, "GBT/Caltech Spigot");
        break;
    case WAPP:
        strcpy(outstr, "WAPP");
        break;
    default:
        strcpy(outstr, "Unknown");
    }
}


void spectra_info_set_defaults(struct spectra_info *s) {
    strcpy(s->telescope, "unset");
    strcpy(s->observer, "unset");
    strcpy(s->source, "unset");
    strcpy(s->frontend, "unset");
    strcpy(s->backend, "unset");
    strcpy(s->project_id, "unset");
    strcpy(s->date_obs, "unset");
    strcpy(s->ra_str, "unset");
    strcpy(s->dec_str, "unset");
    strcpy(s->poln_type, "unset");
    strcpy(s->poln_order, "unset");
    s->datatype = UNSET;
    s->N = 0;
    s->T = 0.0;
    s->dt = 0.0;
    s->fctr = 0.0;
    s->lo_freq = 0.0;
    s->hi_freq = 0.0;
    s->orig_df = 0.0;
    s->chan_dm = 0.0;
    s->df = 0.0;
    s->BW = 0.0;
    s->ra2000 = 0.0;
    s->dec2000 = 0.0;
    s->azimuth = 0.0;
    s->zenith_ang = 0.0;
    s->beam_FWHM = 0.0;
    s->time_per_subint = 0.0;
    s->scan_number = 0;
    s->tracking = 1;
    s->orig_num_chan = 0;
    s->num_channels = 0;
    s->num_polns = 0;
    s->num_beams = 1;
    s->beamnum = 0;
    s->summed_polns = 1;
    s->FITS_typecode = 0;
    s->bits_per_sample = 0;
    s->bytes_per_spectra = 0;
    s->samples_per_spectra = 0;
    s->bytes_per_subint = 0;
    s->spectra_per_subint = 0;
    s->samples_per_subint = 0;
    s->min_spect_per_read = 0;
    s->num_files = 0;
    s->offs_sub_col = 0;
    s->dat_wts_col = 0;
    s->dat_offs_col = 0;
    s->dat_scl_col = 0;
    s->data_col = 0;
    s->apply_scale = 0;
    s->apply_offset = 0;
    s->apply_weight = 0;
    s->apply_flipband = 0;
    s->remote_zeroDM = 0;
    s->use_poln = 0;
    s->flip_bytes = 0;
    s->zero_offset = 0.0;
    s->clip_sigma = 0.0;
    s->start_MJD = NULL;
    s->files = NULL;
    s->fitsfiles = NULL;
    s->header_offset = NULL;
    s->start_subint = NULL;
    s->num_subint = NULL;
    s->start_spec = NULL;
    s->num_spec = NULL;
    s->num_pad = NULL;
};



void print_PSRFITS_info(struct spectra_info *s)
// Output a spectra_info structure in human readable form
{
    int ii, numhdus, hdutype, status = 0, itmp;
    char ctmp[40], comment[120];
    double dtmp;

    printf("From the PSRFITS file '%s':\n", s->fitsfiles[0]->Fptr->filename);
    fits_get_num_hdus(s->fitsfiles[0], &numhdus, &status);
    printf("                       HDUs = primary, ");
    for (ii = 2 ; ii < numhdus + 1 ; ii++) {
        fits_movabs_hdu(s->fitsfiles[0], ii, &hdutype, &status);
        fits_read_key(s->fitsfiles[0], TSTRING, "EXTNAME", ctmp, comment, &status);
        printf("%s%s", ctmp, (ii < numhdus) ? ", " : "\n");
    }
    printf("                  Telescope = %s\n", s->telescope);
    printf("                   Observer = %s\n", s->observer);
    printf("                Source Name = %s\n", s->source);
    printf("                   Frontend = %s\n", s->frontend);
    printf("                    Backend = %s\n", s->backend);
    printf("                 Project ID = %s\n", s->project_id);
    // printf("                Scan Number = %d\n", s->scan_number);
    printf("            Obs Date String = %s\n", s->date_obs);
    DATEOBS_to_MJD(s->date_obs, &itmp, &dtmp);
    sprintf(ctmp, "%.14f", dtmp);
    printf("  MJD start time (DATE-OBS) = %5i.%14s\n", itmp, ctmp+2);
    printf("     MJD start time (STT_*) = %19.14Lf\n", s->start_MJD[0]);
    printf("                   RA J2000 = %s\n", s->ra_str);
    printf("             RA J2000 (deg) = %-17.15g\n", s->ra2000);
    printf("                  Dec J2000 = %s\n", s->dec_str);
    printf("            Dec J2000 (deg) = %-17.15g\n", s->dec2000);
    printf("                  Tracking? = %s\n", s->tracking ? "True" : "False");
    printf("              Azimuth (deg) = %-.7g\n", s->azimuth);
    printf("           Zenith Ang (deg) = %-.7g\n", s->zenith_ang);
    printf("          Polarization type = %s\n", s->poln_type);
    if (s->num_polns>=2 && !s->summed_polns)
        printf("            Number of polns = %d\n", s->num_polns);
    else if (s->summed_polns)
        printf("            Number of polns = 2 (summed)\n");
    else 
        printf("            Number of polns = 1\n");
    printf("         Polarization order = %s\n", s->poln_order);
    printf("           Sample time (us) = %-17.15g\n", s->dt * 1e6);
    printf("         Central freq (MHz) = %-17.15g\n", s->fctr);
    printf("          Low channel (MHz) = %-17.15g\n", s->lo_freq);
    printf("         High channel (MHz) = %-17.15g\n", s->hi_freq);
    printf("        Channel width (MHz) = %-17.15g\n", s->df);
    printf("         Number of channels = %d\n", s->num_channels);
    if (s->chan_dm != 0.0) {
        printf("   Orig Channel width (MHz) = %-17.15g\n", s->orig_df);
        printf("    Orig Number of channels = %d\n", s->orig_num_chan);
        printf("    DM used for chan dedisp = %-17.15g\n", s->chan_dm);
    }
    printf("      Total Bandwidth (MHz) = %-17.15g\n", s->BW);
    printf("         Spectra per subint = %d\n", s->spectra_per_subint);
    printf("            Starting subint = %d\n", s->start_subint[0]);
    printf("           Subints per file = %d\n", s->num_subint[0]);
    printf("           Spectra per file = %lld\n", s->num_spec[0]);
    printf("      Time per subint (sec) = %-.12g\n", s->time_per_subint);
    printf("        Time per file (sec) = %-.12g\n", s->num_spec[0]*s->dt);
    printf("            bits per sample = %d\n", s->bits_per_sample);
    if (s->bits_per_sample < 8)
        itmp = (s->bytes_per_spectra * s->bits_per_sample) / 8;
    else
        itmp = s->bytes_per_spectra;
    printf("          bytes per spectra = %d\n", itmp);
    printf("        samples per spectra = %d\n", s->samples_per_spectra);
    if (s->bits_per_sample < 8)
        itmp = (s->bytes_per_subint * s->bits_per_sample) / 8;
    else
        itmp = s->bytes_per_subint;
    printf("           bytes per subint = %d\n", itmp);
    printf("         samples per subint = %d\n", s->samples_per_subint);
    printf("                zero offset = %-17.15g\n", s->zero_offset);
    printf("           Invert the band? = %s\n", s->apply_flipband ? "True" : "False");

    printf("              FITS typecode = %d\n", s->FITS_typecode);
    printf("                DATA column = %d\n", s->data_col);
    printf("             Apply scaling? = %s\n", s->apply_scale ? "True" : "False");
    printf("             Apply offsets? = %s\n", s->apply_offset ? "True" : "False");
    printf("             Apply weights? = %s\n", s->apply_weight ? "True" : "False");
}


void spectra_info_to_inf(struct spectra_info * s, infodata * idata)
// Convert a spectra_info structure into an infodata structure
{
    char ctmp[100];
    struct passwd *pwd;
    
    strncpy(idata->object, s->source, 80);
    hours2hms(s->ra2000 / 15.0, &(idata->ra_h), &(idata->ra_m), &(idata->ra_s));
    deg2dms(s->dec2000, &(idata->dec_d), &(idata->dec_m), &(idata->dec_s));
    strcpy(idata->telescope, s->telescope);
    strcpy(idata->instrument, s->backend);
    idata->num_chan = s->num_channels;
    idata->dt = s->dt;
    // DATEOBS_to_MJD(s->date_obs, &(idata->mjd_i), &(idata->mjd_f));
    idata->mjd_i = (int)(s->start_MJD[0]);
    idata->mjd_f = s->start_MJD[0] - idata->mjd_i;
    idata->N = s->N;
    idata->freqband = s->BW;
    idata->chan_wid = s->df;
    idata->freq = s->lo_freq;
    idata->fov = s->beam_FWHM * 3600.0; // in arcsec
    idata->bary = 0;
    idata->numonoff = 0;
    strcpy(idata->band, "Radio");
    pwd = getpwuid(geteuid());
    strcpy(idata->analyzer, pwd->pw_name);
    strncpy(idata->observer, s->observer, 80);
    if (s->summed_polns)
        sprintf(ctmp,
                "2 polns were summed.  Samples have %d bits.",
                s->bits_per_sample);
    else
        sprintf(ctmp, "%d polns were not summed.  Samples have %d bits.",
                s->num_polns, s->bits_per_sample);
    sprintf(idata->notes, "Project ID %s, Date: %s.\n    %s\n",
            s->project_id, s->date_obs, ctmp);
}



int read_filterbank_rawblocks(FILE * infiles[], int numfiles,
                              float rawdata[], int numblocks, int *padding)
// This routine reads numblocks filterbank records from the input
// files *infiles.  The floating-point filterbank data is returned in
// rawdata which must have a size of numblocks * s->samples_per_subint.  The
// number of blocks read is returned.  If padding is returned as 1,
// then padding was added and statistics should not be calculated
{
    int ii, retval = 0, pad, numpad = 0;

    *padding = 0;
    for (ii = 0; ii < numblocks; ii++) {
        retval += read_filterbank_rawblock(infiles, numfiles,
                                           rawdata + ii * s->samples_per_subint, &pad);
        if (pad) numpad++;
    }
    /* Return padding 'true' if any block was padding */
    if (numpad) *padding = 1;
    return retval;
}


int read_filterbank(FILE * infiles[], int numfiles, float *data,
                    int numspect, int *delays, int *padding,
                    int *maskchans, int *nummasked, mask * obsmask)
// This routine reads numspect from the filterbank raw files *infiles.
// These files contain raw data in filterbank format.  Time delays and
// a mask are applied to each channel.  It returns the # of points
// read if successful, 0 otherwise.  If padding is returned as 1, then
// padding was added and statistics should not be calculated.
// maskchans is an array of length numchans contains a list of the
// number of channels that were masked.  The # of channels masked is
// returned in nummasked.  obsmask is the mask structure to use for
// masking.
{
   int ii, jj, numread = 0, offset;
   double starttime = 0.0;
   static float *tempzz, *rawdata1, *rawdata2;
   static float *currentdata, *lastdata;
   static int firsttime = 1, numblocks = 1, allocd = 0, mask = 0;
   static double duration = 0.0;

   *nummasked = 0;
   if (firsttime) {
      if (numspect % s->spectra_per_subint) {
         printf("numspect must be a multiple of %d in read_filterbank()!\n",
                s->spectra_per_subint);
         exit(1);
      } else
         numblocks = numspect / s->spectra_per_subint;
      if (obsmask->numchan)
         mask = 1;
      rawdata1 = gen_fvect(numblocks * s->samples_per_subint);
      rawdata2 = gen_fvect(numblocks * s->samples_per_subint);
      allocd = 1;
      duration = numblocks * s->time_per_subint;
      currentdata = rawdata1;
      lastdata = rawdata2;
   }

   /* Read, convert and de-disperse */
   if (allocd) {
      while (1) {
         numread = read_filterbank_rawblocks(infiles, numfiles, currentdata,
                                             numblocks, padding);
         if (mask) {
            starttime = currentblock * s->time_per_subint;
            *nummasked = check_mask(starttime, duration, obsmask, maskchans);
         }

         /* Clip nasty RFI if requested and we're not masking all the channels */
         if ((clip_sigma_st > 0.0) && !(mask && (*nummasked == -1)))
            clip_times(currentdata, numspect, s->num_channels, clip_sigma_st, padvals);

         if (mask) {
            if (*nummasked == -1) {     /* If all channels are masked */
               for (ii = 0; ii < numspect; ii++)
                   memcpy(currentdata + ii * s->num_channels, 
                          padvals, s->num_channels * sizeof(float));
            } else if (*nummasked > 0) {        /* Only some of the channels are masked */
               int channum;
               for (ii = 0; ii < numspect; ii++) {
                  offset = ii * s->num_channels;
                  for (jj = 0; jj < *nummasked; jj++) {
                     channum = maskchans[jj];
                     currentdata[offset + channum] = padvals[channum];
                  }
               }
            }
         }
         if (!firsttime)
             float_dedisp(currentdata, lastdata, numspect, s->num_channels, delays, 0.0, data);

         SWAP(currentdata, lastdata);
         if (numread != numblocks) {
            vect_free(rawdata1);
            vect_free(rawdata2);
            allocd = 0;
         }
         if (firsttime)
            firsttime = 0;
         else
            break;
      }
      return numblocks * s->spectra_per_subint;
   } else {
      return 0;
   }
}


void get_filterbank_channel(int channum, float chandat[],
                            float rawdata[], int numblocks)
// Return the values for channel 'channum' of a block of 'numblocks'
// filterbank floating-point data stored in 'rawdata' in 'chandat'.
// 'rawdata' should have been initialized using
// read_filterbank_rawblocks(), and 'chandat' must have at least
// 'numblocks' * 's->spectra_per_subint' spaces.  Channel 0 is assumed to be
// the lowest freq channel.
{
   int ii, jj;

   if (channum > s->num_channels || channum < 0) {
      printf("\nchannum = %d is out of range in get_GMR_channel()!\n\n", channum);
      exit(1);
   }
   /* Select the correct channel */
   for (ii = 0, jj = channum; ii < numblocks * s->spectra_per_subint; ii++, jj += s->num_channels)
      chandat[ii] = rawdata[jj];
}


int prep_filterbank_subbands(float *rawdata, float *data,
                             int *delays, int numsubbands,
                             int transpose, int *maskchans,
                             int *nummasked, mask * obsmask)
// This routine preps a block from from the filterbank system.  It
// uses dispersion delays in 'delays' to de-disperse the data into
// 'numsubbands' subbands.  It stores the resulting data in vector
// 'data' of length 'numsubbands' * 's->spectra_per_subint'.  The low freq
// subband is stored first, then the next highest subband etc, with
// 's->spectra_per_subint' floating points per subband. It returns the # of
// points read if succesful, 0 otherwise.  'maskchans' is an array of
// length numchans which contains a list of the number of channels
// that were masked.  The # of channels masked is returned in
// 'nummasked'.  'obsmask' is the mask structure to use for masking.
// If 'transpose'==0, the data will be kept in time order instead of
// arranged by subband as above.
{
   int ii, jj, trtn, offset;
   double starttime = 0.0;
   static float *tempzz;
   static float rawdata1[MAXNUMCHAN * BLOCKLEN],
       rawdata2[MAXNUMCHAN * BLOCKLEN];
   static float *currentdata, *lastdata;
   static unsigned char *move;
   static int firsttime = 1, move_size = 0, mask = 0;

   *nummasked = 0;
   if (firsttime) {
      if (obsmask->numchan)
         mask = 1;
      move_size = (s->spectra_per_subint + numsubbands) / 2;
      move = gen_bvect(move_size);
      currentdata = rawdata1;
      lastdata = rawdata2;
   }

   /* Read and de-disperse */
   memcpy(currentdata, rawdata, s->samples_per_subint * sizeof(float));
   if (mask) {
      starttime = currentblock * s->time_per_subint;
      *nummasked = check_mask(starttime, s->time_per_subint, 
                              obsmask, maskchans);
   }

   /* Clip nasty RFI if requested and we're not masking all the channels*/
   if ((clip_sigma_st > 0.0) && !(mask && (*nummasked == -1)))
      clip_times(currentdata, s->spectra_per_subint, s->num_channels, clip_sigma_st, padvals);

   if (mask) {
      if (*nummasked == -1) {   /* If all channels are masked */
         for (ii = 0; ii < s->spectra_per_subint; ii++)
             memcpy(currentdata + ii * s->num_channels, 
                    padvals, s->num_channels * sizeof(float));
      } else if (*nummasked > 0) {      /* Only some of the channels are masked */
         int channum;
         for (ii = 0; ii < s->spectra_per_subint; ii++) {
            offset = ii * s->num_channels;
            for (jj = 0; jj < *nummasked; jj++) {
               channum = maskchans[jj];
               currentdata[offset + channum] = padvals[channum];
            }
         }
      }
   }

   /* In mpiprepsubband, the nodes do not call read_*_rawblock() */
   /* where currentblock gets incremented.                       */
   if (using_MPI) currentblock++;

   if (firsttime) {
      SWAP(currentdata, lastdata);
      firsttime = 0;
      return 0;
   } else {
      dedisp_subbands(currentdata, lastdata, s->spectra_per_subint, s->num_channels,
                      delays, numsubbands, data);
      SWAP(currentdata, lastdata);
      /* Transpose the data into vectors in the result array */
      if (transpose) {
         if ((trtn = transpose_float(data, s->spectra_per_subint, numsubbands,
                                     move, move_size)) < 0)
            printf("Error %d in transpose_float().\n", trtn);
      }
      return s->spectra_per_subint;
   }
}


int read_filterbank_subbands(FILE *infiles[], int numfiles, float *data,
                             int *delays, int numsubbands,
                             int transpose, int *padding,
                             int *maskchans, int *nummasked, mask * obsmask)
// This routine reads a record from the input files *infiles[] in
// SIGPROC filterbank format.  The routine uses dispersion delays in
// 'delays' to de-disperse the data into 'numsubbands' subbands.
// It stores the resulting data in vector 'data' of length
// 'numsubbands' * 's->spectra_per_subint'.  The low freq subband is stored
// first, then the next highest subband etc, with 's->spectra_per_subint'
// floating points per subband.  It returns the # of points read if
// succesful, 0 otherwise. If padding is returned as 1, then padding
// was added and statistics should not be calculated.  'maskchans' is
// an array of length numchans which contains a list of the number of
// channels that were masked.  The # of channels masked is returned in
// 'nummasked'.  'obsmask' is the mask structure to use for masking.
// If 'transpose'==0, the data will be kept in time order instead of
// arranged by subband as above.
{
   static int firsttime = 1;
   static float rawdata[MAXNUMCHAN * BLOCKLEN];

   if (firsttime) {
      if (!read_filterbank_rawblock(infiles, numfiles, rawdata, padding)) {
         printf("Problem reading the raw filterbank data file.\n\n");
         return 0;
      }
      if (0 != prep_filterbank_subbands(rawdata, data, delays, numsubbands,
                                        transpose, maskchans, nummasked, obsmask)) {
         printf("Problem initializing prep_filterbank_subbands()\n\n");
         return 0;
      }
      firsttime = 0;
   }
   if (!read_filterbank_rawblock(infiles, numfiles, rawdata, padding)) {
      /* printf("Problem reading the raw filterbank data file.\n\n"); */
      return 0;
   }
   return prep_filterbank_subbands(rawdata, data, delays, numsubbands,
                                   transpose, maskchans, nummasked, obsmask);
}



void filterbank_update_infodata(int numfiles, infodata * idata)
// Update the onoff bins section in case we used multiple files
{

   int ii, index = 2;

   idata->N = N_st;
   if (numfiles == 1 && s->num_pad[0] == 0) {
      idata->numonoff = 0;
      return;
   }
   /* Determine the topocentric onoff bins */
   idata->numonoff = 1;
   idata->onoff[0] = 0.0;
   idata->onoff[1] = s->num_spec[0] - 1.0;
   for (ii = 1; ii < numfiles; ii++) {
      if (s->num_pad[ii - 1]) {
         idata->onoff[index] = idata->onoff[index - 1] + s->num_pad[ii - 1];
         idata->onoff[index + 1] = idata->onoff[index] + s->num_spec[ii];
         idata->numonoff++;
         index += 2;
      } else {
         idata->onoff[index - 1] += s->num_spec[ii];
      }
   }
   if (s->num_pad[numfiles - 1]) {
      idata->onoff[index] = idata->onoff[index - 1] + s->num_pad[numfiles - 1];
      idata->onoff[index + 1] = idata->onoff[index];
      idata->numonoff++;
   }
}

