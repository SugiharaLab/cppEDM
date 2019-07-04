
#---------------------------------------------------------------------
#---------------------------------------------------------------------
Run = function( path = './' ) {
  PlotSimplexSmap( path )
  PlotSMapCircle ( path )
  PlotCCM        ( path )
  PlotEval       ( path )
}

#---------------------------------------------------------------------
#---------------------------------------------------------------------
Clean = function( path = './' ) {
  files = c( "Test",
             "cppBlock3sp_E3.csv",
             "cppBlock3sp_Embedded.csv",
             "smap_3sp_Embed.csv",
             "smap_3sp_coeff.csv",
             "smap_circ_coeff.csv",
             "smap_circle.csv",
             "ccm.csv",
             "EmbedDimOut.csv",
             "PredictIntervalOut.csv",
             "PredictNonlinearOut.csv",
             "MultiviewBlock3sp.csv" )

  file.remove( files )
}

#---------------------------------------------------------------------
#---------------------------------------------------------------------
PlotSimplexSmap = function( path = './' ) {

  if ( is.null( dev.list() ) ) {
    dev.new()
    par( mar = c(2, 3.8, 0.5, 1), mgp = c(2.2, 0.8, 0), cex = 1.3, 
         cex.axis = 1.5, cex.lab = 1.5, mfrow = c(7, 1) )
  }

  f1 = "cppBlock3sp_E3.csv"
  f2 = "cppBlock3sp_Embedded.csv"
  f3 = "smap_3sp_Embed.csv"
  f4 = "smap_3sp_coeff.csv"
  f5 = "MultiviewBlock3sp.csv" # as.is = T since last times can be string

  df1 = read.csv( paste( path, f1, sep = '' ), na.string = 'nan' )
  df2 = read.csv( paste( path, f2, sep = '' ), na.string = 'nan' )
  df3 = read.csv( paste( path, f3, sep = '' ), na.string = 'nan' )
  df4 = read.csv( paste( path, f4, sep = '' ), na.string = 'nan' )
  df5 = read.csv( paste( path, f5, sep = '' ), na.string = 'nan', as.is = TRUE )

  # Simplex: cppBlock3sp_E3.csv
  plot ( df1$time, df1$Observations, type = 'l', lwd = 3, ylab = '' )
  lines( df1$time, df1$Predictions, col = 'red', lwd = 3 )
  abline( h = 0 )
  corcoef = round( cor( df1$Observations, df1$Predictions,
                        use = "pairwise.complete.obs" ), 2 )
  mtext( paste( " Simplex E=3  ρ =", corcoef ),
         side = 3, line = -1.5, adj = 0, cex = 1.3 )
  legend( 'topright', horiz = TRUE, legend = c( "Obs", "Simplex" ),
          fill = c( 'black', 'red' ), cex = 1.5, bg = 'white' )

  # Simplex: cppBlock3sp_Embedded.csv
  plot ( df2$time, df2$Observations, type = 'l', lwd = 3, ylab = '' )
  lines( df2$time, df2$Predictions, col = 'red', lwd = 3 )
  abline( h = 0 )
  corcoef = round( cor( df2$Observations, df2$Predictions,
                        use = "pairwise.complete.obs" ), 2 )
  mtext( paste( " Simplex embedded  ρ =", corcoef ),
         side = 3, line = -1.5, adj = 0, cex = 1.3 )
  legend( 'topright', horiz = TRUE, legend = c( "Obs", "Simplex" ),
          fill = c( 'black', 'red' ), cex = 1.5, bg = 'white' )

  # Multiview: MultiviewBlock3sp.csv
  plot ( df5$time, df5$Observations, type = 'l', lwd = 3, ylab = '' )
  lines( df5$time, df5$Predictions, col = 'red', lwd = 3 )
  abline( h = 0 )
  corcoef = round( cor( df5$Observations, df5$Predictions,
                        use = "pairwise.complete.obs" ), 2 )
  mtext( paste( " Multiview   ρ =", corcoef ),
         side = 3, line = -1.5, adj = 0, cex = 1.3 )
  legend( 'topright', horiz = TRUE, legend = c( "Obs", "Pred" ),
          fill = c( 'black', 'red' ), cex = 1.5, bg = 'white' )

  # SMap: smap_3sp_Embed.csv
  plot ( df3$time, df3$Observations, type = 'l', lwd = 3, ylab = '' )
  lines( df3$time, df3$Predictions, col = 'red', lwd = 3 )
  abline( h = 0 )
  corcoef = round( cor( df3$Observations, df3$Predictions,
                        use = "pairwise.complete.obs" ), 2 )
  mtext( paste( " SMap embedded  ρ =", corcoef ),
         side = 3, line = -1.5, adj = 0, cex = 1.3 )
  legend( 'topright', horiz = TRUE, legend = c( "Obs", "SMap" ),
          fill = c( 'black', 'red' ), cex = 1.5, bg = 'white' )

  # SMap: smap_3sp_coeff.csv
  plot ( df4$time, df4$C0, type = 'l', lwd = 3, ylab = 'SMap C0' )
  abline( h = 0 )
  plot ( df4$time, df4$C1, type = 'l', lwd = 3, ylab = 'SMap C1' )
  abline( h = 0 )
  plot ( df4$time, df4$C2, type = 'l', lwd = 3, ylab = 'SMap C2' )
  abline( h = 0 )  
} 

#---------------------------------------------------------------------
#---------------------------------------------------------------------
PlotSMapCircle = function( path = './' ) {
  
  if ( is.null( dev.list() ) || length( dev.list() ) == 1 ) {
    dev.new()
    par( mar = c(2, 3.8, 0.5, 1), mgp = c(2.2, 0.8, 0), cex = 1.3, 
         cex.axis = 1.5, cex.lab = 1.5, mfrow = c(3, 1) )
  }

  c = read.csv( paste( path, 'smap_circ_coeff.csv', sep = '' ))
  p = read.csv( paste( path, 'smap_circle.csv',     sep = '' ))
  
  plot( p$Time, p$Observations, type='l', lwd=3, xlab='Time', ylab='x')
  lines( p$Time, p$Predictions, lwd=3, col = 'red' )
  legend('topright',legend=c('Observed','Predicted'),
          cex = 1.7, fill=c('black','red'))
  
  plot(c$Time,c$C0,type='l',lwd=3, xlab='Time', ylab='S Map C0')
  mtext(line=-2,"C0",cex=1.6)
  
  plot(c$Time,c$C1,type='l',lwd=3,ylim=range(c$C1,c$C2),
       col='red',xlab='Time',ylab='S Map Coef')
  lines( c$Time, c$C2, lwd=3, col='blue')
  legend('top',legend=c('C1','C2'),fill=c('red','blue'),cex=1.7)

}

#---------------------------------------------------------------------
#---------------------------------------------------------------------
PlotCCM = function(
  path     = './',
  file     = 'ccm.csv',
  col_i    = 2,
  target_i = 3
) {
  
  if ( is.null( dev.list() ) || length( dev.list() ) == 2 ) {
    dev.new()
    par( mar = c(3.5, 3.8, 0.5, 1), mgp = c(2.2, 0.8, 0), cex = 1.3, 
         cex.axis = 1.5, cex.lab = 1.5, mfrow = c(1, 1) )
  }

  c = read.csv( paste( path, file, sep = '' ))

  # LibSize,anchovy:np_sst,np_sst:anchovy
  plot( c $ LibSize, c[,col_i], type='l', lwd=3,
        xlab='Library Size', ylab='rho', col = 'blue',
        ylim = range( c[,col_i],c[,target_i] ) )
  lines( c $ LibSize, c[,target_i], lwd=3, col = 'red' )
  abline( h = 0 )
  legend( 'topleft', legend = c(names(c)[col_i], names(c)[target_i] ),
          cex = 1.5, fill=c('blue','red') )
}

#---------------------------------------------------------------------
#---------------------------------------------------------------------
PlotEval = function(
  path = './'
) {
  
  if ( is.null( dev.list() ) || length( dev.list() ) == 3 ) {
    dev.new()
    par( mar = c(3.5, 3.8, 0.5, 1), mgp = c(2.2, 0.8, 0), cex = 1.3, 
         cex.axis = 1.5, cex.lab = 1.5, mfrow = c(3, 1) )
  }

  E.rho     = read.csv( paste( path, "EmbedDimOut.csv",         sep = '' ) )
  Tp.rho    = read.csv( paste( path, "PredictIntervalOut.csv",  sep = '' ) )
  Theta.rho = read.csv( paste( path, "PredictNonlinearOut.csv", sep = '' ) )
  
  #----------------------------------------------------
  plot( E.rho $ E, E.rho $ rho, type='l', lwd = 3,
        xlab='E', ylab='rho', col = 'blue' )

  plot( Tp.rho $ Tp, Tp.rho $ rho, type='l', lwd = 3,
        xlab='Tp', ylab='rho', col = 'blue' )

  plot( Theta.rho $ Theta, Theta.rho $ rho, type='l', lwd = 3,
        xlab='Theta', ylab='rho', col = 'blue' )

}
