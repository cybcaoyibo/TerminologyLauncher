﻿using System;
using System.Collections.Generic;
using System.IO;
using TerminologyLauncher.Auth;
using TerminologyLauncher.Configs;
using TerminologyLauncher.Core.Handlers;
using TerminologyLauncher.Core.Handlers.LoginHandlers;
using TerminologyLauncher.Core.Handlers.MainHandlers;
using TerminologyLauncher.Entities.Account;
using TerminologyLauncher.Entities.FileRepository;
using TerminologyLauncher.FileRepository;
using TerminologyLauncher.GUI;
using TerminologyLauncher.Logging;

namespace TerminologyLauncher.Core
{
    public class Engine
    {
        #region Instance

        public static Engine Instance;

        public static Engine GetEngine()
        {
            return Instance ?? (Instance = new Engine());
        }

        #endregion
        public Config CoreConfig { get; set; }
        public UiControl UiControl { get; set; }
        public AuthServer AuthServer { get; set; }
        public FileRepository.FileRepository FileRepo { get; set; }
        public InstanceManager.InstanceManager InstanceManager { get; set; }
        public PlayerEntity CurrentPlayer { get; set; }
        public Dictionary<String, HandlerBase> Handlers { get; set; }
        public Engine()
        {
            Logger.GetLogger().Info("Engine Initializing...");
            this.CoreConfig = new Config(new FileInfo("Configs/CoreConfig.json"));
            this.UiControl = new UiControl();
            this.AuthServer = new AuthServer(this.CoreConfig.GetConfig("authConfig"));

            this.Handlers = new Dictionary<string, HandlerBase>();
            Logger.GetLogger().Info("Engine Initialized!");
        }
        public void Run()
        {
            this.RegisterFixedEvents();
            Logger.GetLogger().Info("Engine running...");
            this.UiControl.ShowLoginWindow();
            Logger.GetLogger().Info("Starting GUI...");
            this.UiControl.Run();
            Logger.GetLogger().Info("Exit running.");
        }

        public void Exit()
        {
            Logger.GetLogger().Info("Engine shutting down...");
        }

        public void RegisterFixedEvents()
        {
            Logger.GetLogger().Debug("Engine Register events.");
            //DONE:Using IHandler interface, let handlers register their events duding ctor.
            this.Handlers.Add("WINDOWS_CLOSE", new CloseHandler(this));
            this.Handlers.Add("LOGIN", new LoginHandlerBase(this));
            this.Handlers.Add("LOGIN_WINDOW_VISIBILITY_CHANGED", new LoginWindowVisibilityChangedHandler(this));
            this.Handlers.Add("MAIN_WINDOW_VISIBILITY_CHANGED", new MainWindowVisibilityChangedHandler(this));
        }

        public void InitializeComponents()
        {
            Logger.GetLogger().Info("Engine extra component initializing...");
            this.FileRepo = new FileRepository.FileRepository(this.CoreConfig.GetConfig("fileRepositoryConfig"));
            this.InstanceManager = new InstanceManager.InstanceManager();
            Logger.GetLogger().Info("Engine extra component initialized...");
        }
    }
}
