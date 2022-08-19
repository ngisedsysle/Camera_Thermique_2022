// #include "thermal_camera.h"

// static const uint16_t g_daysbeforemonth[13] =
// {
//   0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
// };

// int clock_isleapyear(int year)
// {
//   return (year % 400) ? ((year % 100) ? ((year % 4) ? 0 : 1) : 0) : 1;
// }

// int clock_daysbeforemonth(int month, bool leapyear)
// {
//   int retval = g_daysbeforemonth[month];
//   if (month >= 2 && leapyear)
//     {
//       retval++;
//     }

//   return retval;
// }

// void 
// clock_utc2calendar(time_t days, FAR int *year, FAR int *month,
//                                FAR int *day)
// {
//   int  value;
//   int  min;
//   int  max;
//   int  tmp;
//   bool leapyear;

//   /* There is one leap year every four years, so we can get close with the
//    * following:
//    */

//   value   = days  / (4 * 365 + 1); /* Number of 4-years periods since the epoch */
//   caminfo("value 1 = %d\n", value);
//   days   -= value * (4 * 365 + 1); /* Remaining days */
//   value <<= 2;                     /* Years since the epoch */

//   /* Then we will brute force the next 0-3 years
//    *
//    * Is this year a leap year? (we'll need this later too)
//    */

//   leapyear = clock_isleapyear(value + 1992);

//   /* Get the number of days in the year */

//   tmp = (leapyear ? 366 : 365);
//   caminfo("tmp = %d\n", tmp);

//   /* Do we have that many days left to account for? */

//   while (days >= tmp)
//     {
//       /* Yes.. bump up the year and subtract the number of days in the year */
//       caminfo("days remaining = %d\n", days);
//       value++;
//         caminfo("value = %d\n", value);
//       days -= tmp;

//       /* Is the next year a leap year? */

//       leapyear = clock_isleapyear(value + 1992);

//       /* Get the number of days in the next year */

//       tmp = (leapyear ? 366 : 365);
//         caminfo("tmp = %d\n", tmp);
//     }

//   /* At this point, 'value' has the years since 1970 and 'days' has number
//    * of days into that year.  'leapyear' is true if the year in 'value' is
//    * a leap year.
//    */

//   *year = 1992 + value;
//     caminfo("year = %d\n", *year);

//   /* Handle the month (zero based) */

//   min = 0;
//   max = 11;

//   do
//     {
//       /* Get the midpoint */

//       value = (min + max) >> 1;

//       /* Get the number of days that occurred before the beginning of the
//        * month following the midpoint.
//        */

//       tmp = clock_daysbeforemonth(value + 1, leapyear);

//       /* Does the number of days before this month that equal or exceed the
//        * number of days we have remaining?
//        */

//       if (tmp > days)
//         {
//           /* Yes.. then the month we want is somewhere from 'min' and to the
//            * midpoint, 'value'.  Could it be the midpoint?
//            */

//           tmp = clock_daysbeforemonth(value, leapyear);
//           if (tmp > days)
//             {
//               /* No... The one we want is somewhere between min and value-1 */

//               max = value - 1;
//             }
//           else
//             {
//               /* Yes.. 'value' contains the month that we want */

//               break;
//             }
//         }
//       else
//         {
//           /* No... The one we want is somewhere between value+1 and max */

//           min = value + 1;
//         }

//       /* If we break out of the loop because min == max, then we want value
//        * to be equal to min == max.
//        */

//       value = min;
//     }
//   while (min < max);

//   /* The selected month number is in value. Subtract the number of days in
//    * the selected month
//    */

//   days -= clock_daysbeforemonth(value, leapyear);

//   /* At this point, value has the month into this year (zero based) and days
//    * has number of days into this month (zero based)
//    */

//   *month = value + 1; /* 1-based */
//   *day   = days + 1;  /* 1-based */

// }
