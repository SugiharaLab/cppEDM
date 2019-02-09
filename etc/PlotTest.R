
#---------------------------------------------------------------------
#---------------------------------------------------------------------
Run = function( path = './' ) {
  PlotSimplexSmap( path )
  PlotSMapCircle( path )
}
 
#---------------------------------------------------------------------
#---------------------------------------------------------------------
PlotSimplexSmap = function( path = './' ) {

  if ( is.null( dev.list() ) ) {
    dev.new()
    par( mar = c(2, 3.8, 0.5, 1), mgp = c(2.2, 0.8, 0), cex = 1.3, 
         cex.axis = 1.5, cex.lab = 1.5, mfrow = c(5, 1) )
  }

  f1 = "simplex_3sp.csv"
  f2 = "simplex_3sp_Embed.csv"
  f3 = "smap_3sp_Embed.csv"
  f4 = "smap_3sp_coeff.csv"   

  df1 = read.csv( paste( path, f1, sep = '/' ) )
  df2 = read.csv( paste( path, f2, sep = '/' ) )
  df3 = read.csv( paste( path, f3, sep = '/' ) )
  df4 = read.csv( paste( path, f4, sep = '/' ) )

  plot ( df1$Time, df1$Observations, type = 'l', lwd = 3, ylab = '' )
  lines( df1$Time, df1$Predictions, col = 'red', lwd = 3 )
  legend( 'topright', horiz = TRUE, legend = c( "Obs", "Simplex" ),
          fill = c( 'black', 'red' ), cex = 1.5, bg = 'white' )

  plot ( df2$Time, df2$Observations, type = 'l', lwd = 3, ylab = '' )
  lines( df2$Time, df2$Predictions, col = 'red', lwd = 3 )
  legend( 'topright', horiz = TRUE, legend = c( "Obs", "Simplex" ),
          fill = c( 'black', 'red' ), cex = 1.5, bg = 'white' )

  plot ( df3$Time, df3$Observations, type = 'l', lwd = 3, ylab = '' )
  lines( df3$Time, df3$Predictions, col = 'red', lwd = 3 )
  legend( 'topright', horiz = TRUE, legend = c( "Obs", "SMap" ),
          fill = c( 'black', 'red' ), cex = 1.5, bg = 'white' )

  plot ( df4$Time, df4$C1, type = 'l', lwd = 3, ylab = 'SMap C1' )
  plot ( df4$Time, df4$C2, type = 'l', lwd = 3, ylab = 'SMap C2' )
  
} 

#---------------------------------------------------------------------
#---------------------------------------------------------------------
PlotSMapCircle = function( path = './' ) {
  
  if ( is.null( dev.list() ) || length( dev.list() ) == 1 ) {
    dev.new()
    par( mar = c(2, 3.8, 0.5, 1), mgp = c(2.2, 0.8, 0), cex = 1.3, 
         cex.axis = 1.5, cex.lab = 1.5, mfrow = c(3, 1) )
  }

  c = read.csv( paste( path, 'smap_circ_coeff.csv', sep = '/' ))
  p = read.csv( paste( path, 'smap_circle.csv',     sep = '/' ))
  
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