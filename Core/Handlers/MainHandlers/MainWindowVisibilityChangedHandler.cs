﻿using System;
using System.Windows;
using TerminologyLauncher.Logging;

namespace TerminologyLauncher.Core.Handlers.MainHandlers
{
    public class MainWindowVisibilityChangedHandler : IHandler
    {
        public void HandleEvent(Object sender, EventArgs e)
        {
            throw new NotSupportedException();
        }

        public void HandleEvent(object sender, DependencyPropertyChangedEventArgs e)
        {
            Logger.GetLogger().Debug("Handle main show or hide at here.");
            var window = (Window)sender;
            Logger.GetLogger().Debug(String.Format("Main window changed status to {0}", window.Visibility));
            //TODO:clear or load data at here.
            switch (window.Visibility)
            {
                case Visibility.Hidden:
                    {

                        break;
                    }
                case Visibility.Visible:
                    {
                        break;
                    }
                default:
                    {
                        Logger.GetLogger().Error(String.Format("Handler could not handle {0} status.", window.Visibility));
                        break;
                    }
            }
            return;
        }
    }
}