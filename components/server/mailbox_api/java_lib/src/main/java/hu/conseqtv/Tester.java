package hu.conseqtv;


public class Tester{
    private static TrafficLight trafficLight;

    public static void main(String[] args) {
        trafficLight = new TrafficLight();
        trafficLight.registerErrorHandler((s) -> {System.err.println(s); System.exit(-1);});

        try {
            testSetState();
            System.out.println("setState works as expected.");
            testSendSignal();
            System.out.println("sendSignal works as expected.");
            testSetTimeA();
            System.out.println("setTimeA works as expected.");
            testSetTimeB();
            System.out.println("setTimeB works as expected.");
            testSetTimeC();
            System.out.println("setTimeC works as expected.");
            testSetTimeC_();
            System.out.println("setTimeC_ works as expected.");
            testSetTimeD();
            System.out.println("setTimeD works as expected.");
            testSetTimeE();
            System.out.println("setTimeE works as expected.");
            testSetTimeF();
            System.out.println("setTimeF works as expected.");
            testSetTimeG();
            System.out.println("setTimeG works as expected.");
            testSetTimeH();
            System.out.println("setTimeH works as expected.");
            testLedRed();
            System.out.println("Red led works as expected.");
            testLedGreen();
            System.out.println("Green led works as expected.");
            testLedYellow();
            System.out.println("Yellow led works as expected.");
            testLedPedRed();
            System.out.println("Pedestrian red led works as expected.");
            testLedPedGreen();
            System.out.println("Pedestrian green led works as expected.");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

    private static void testSetState() throws InterruptedException {
        for(String state : TrafficLight.getStates()) {
            trafficLight.setState(state);
            if(!trafficLight.getState().equals(state)) {
                System.out.println("Could not set state \"" + state + "\". State is: \"" + trafficLight.getState()  + "\" Exiting.\n");
                System.exit(-1);
            }
        }
    }

    private static void testSendSignal() throws InterruptedException {
        String currentState;
        trafficLight.setState(TrafficLight.getStates().get(0));
        trafficLight.sendSignal(TrafficLight.getSignals().get(0));

        if(!(currentState = trafficLight.getState()).equals(TrafficLight.getStates().get(1))) {
            System.out.println("Could not send signal \"" + TrafficLight.getSignals().get(0) + "\". Current state is \"" + currentState  +  "\". Exiting.\n");
            System.exit(-1);
        }
        trafficLight.sendSignal(TrafficLight.getSignals().get(2));

        if(!(currentState = trafficLight.getState()).equals(TrafficLight.getStates().get(2))) {
            System.out.println("Could not send signal \"" + TrafficLight.getSignals().get(2) + "\". Current state is \"" + currentState  +  "\". Exiting.\n");
            System.exit(-1);
        }
        trafficLight.sendSignal(TrafficLight.getSignals().get(1));

        if(!(currentState = trafficLight.getState()).equals(TrafficLight.getStates().get(0))) {
            System.out.println("Could not send signal \"" + TrafficLight.getSignals().get(1) + "\". Current state is \"" + currentState  +  "\". Exiting.\n");
            System.exit(-1);
        }
    }

    private static final Integer[] timeTestData = {500, 1000, 3000, 5000, 10000};

    private static void testSetTimeA() throws InterruptedException {
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeA(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(2));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(3))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time A to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.\n");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeB(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeB(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(3));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(4))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time B to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeC(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeC(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(4));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(6))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time C to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeC_(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeC_(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(5));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(6))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time C_ to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeD(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeD(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(6));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(7))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time D to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeE(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeE(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(7));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(8))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time E to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeF(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeF(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(8));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(9))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time F to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeG(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeG(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(9));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(10))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time G to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeH(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeH(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(10));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(2))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time H to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void testLedRed(){
        if(0 == trafficLight.testRed()) {
            System.out.println("Physical connection problem with the red led.");
            System.exit(-1);
        }
    }
    private static void testLedYellow(){
        if(0 == trafficLight.testYlw()) {
            System.out.println("Physical connection problem with the yellow led.");
            System.exit(-1);
        }
    }
    private static void testLedGreen(){
        if(0 == trafficLight.testGrn()) {
            System.out.println("Physical connection problem with the green led.");
            System.exit(-1);
        }
    }
    private static void testLedPedRed(){
        if(0 == trafficLight.testPred()) {
            System.out.println("Physical connection problem with the pedestrian red led.");
            System.exit(-1);
        }
    }
    private static void testLedPedGreen(){
        if(0 == trafficLight.testPgrn()) {
            System.out.println("Physical connection problem with the pedestrian green led.");
            System.exit(-1);
        }
    }

}